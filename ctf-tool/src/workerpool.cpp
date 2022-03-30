#include <ctf-tool/operations/operations.h>
#include <ctf-tool/terminal.h>
#include <ctf-tool/workerpool.h>

#include <functional>

namespace ctf {
#define CREATE_OP_SWITCH(op)     \
    case AllOperations::Op_##op: \
        return std::make_shared<OpCls_##op>(input, m_config.get_key());

std::shared_ptr<WorkerPool> WorkerPool::get_instance(const Config &config) {
    return std::shared_ptr<WorkerPool>(new WorkerPool(config));
}

WorkerPool::WorkerPool(const Config &config) : m_config(config) {}

WorkerPool::~WorkerPool() {
    stop();
}

void WorkerPool::solve() {
    for (int i = 0; i < m_config.get_num_workers(); i++) {
        m_workers.push_back(std::make_shared<Worker>(i, shared_from_this()));
    }
    m_completed_jobs.resize(m_config.get_num_workers(), nullptr);  // each worker has a spot
    m_assigned_jobs.resize(m_config.get_num_workers(), nullptr);

    // first: add all operations to the queue
    add_operations_to_queue(nullptr, m_config.get_input());

    // second: create all threads
    for (auto &worker : m_workers) {
        // create a thread with the run function
        std::thread thread(std::bind(&Worker::run, worker));
        thread.detach();
    }

    Terminal::print_log_message("[!] Booted up " + std::to_string(m_config.get_num_workers()) + " workers");
    Terminal::print_log_message("[!] Flag Format: \"" + m_config.get_format().get_format() + "\"");
    Terminal::print_log_message("[!] Key: \"" + m_config.get_key().get_key() + "\"");
    Terminal::print_log_message("[!] Input: \"" + m_config.get_input().get_input() + "\"");

    int loop_count = 0;
    while (has_open_work()) {
        // check a thread for "open-to-work"
        for (int i = 0; i < m_config.get_num_workers(); i++) {
            if (m_assigned_jobs[i] == nullptr) {
                auto operation = get_next_job();
                if (operation) {
                    operation->setAssigned();
                    // assign the job to the worker
                    m_assigned_jobs[i] = operation;
                    Terminal::print_worker_message("[" + std::to_string(i) + "] Worker got job: \"" + AllOperations_to_string(operation->type()) + "\"");
                }
            }

            if (m_assigned_jobs[i] == m_completed_jobs[i] && m_completed_jobs[i] != nullptr) {
                // the job is complete
                Terminal::print_worker_message("[" + std::to_string(i) + "] Worker completed job: \"" + AllOperations_to_string(m_completed_jobs[i]->type()) +
                                               "\"");
                m_assigned_jobs[i] = nullptr;
                mark_as_complete(m_completed_jobs[i]);
                m_completed_jobs[i] = nullptr;

                if (is_stopped)
                    return;
            }
        }

        if (loop_count++ % 100 == 0) {
            // count the number of completed jobs
            int num_completed = 0;
            int num_running = 0;
            int num_not_started = 0;
            for (auto &job : m_jobs)
                switch (job->status()) {
                    case OperationStatus::NotStarted:
                        num_not_started++;
                        break;
                    case OperationStatus::Running:
                        num_running++;
                        break;
                    case OperationStatus::Finished:
                        num_completed++;
                        break;
                }

            Terminal::print_pool_message("[!] Has a total of " + std::to_string(m_jobs.size()) + " jobs in the queue");
            Terminal::print_pool_message("[!] " + std::to_string(num_completed) + " completed; " + std::to_string(num_running) + " running; " +
                                         std::to_string(num_not_started) + " not started");
            Terminal::print_all();  // print all the logs
        }
    }

    stop();
}

std::shared_ptr<Operation> WorkerPool::get_next_job() {
    std::lock_guard<std::mutex> lock(m_jobs_mutex);
    for (auto &job : m_jobs) {
        if (job->status() == OperationStatus::NotStarted) {
            return job;
        }
    }
    return nullptr;
}

bool WorkerPool::has_open_work() const {
    std::lock_guard<std::mutex> lock(m_jobs_mutex);

    // check if any job is still in running / waiting state
    for (auto &job : m_jobs) {
        if (job->status() != OperationStatus::Finished) {
            return true;
        }
    }

    for (int i = 0; i < m_assigned_jobs.size(); i++) {
        if (m_assigned_jobs[i] != nullptr) {
            return true;
        }
    }

    for (int i = 0; i < m_completed_jobs.size(); i++) {
        if (m_completed_jobs[i] != nullptr) {
            return true;
        }
    }

    return false;
}

void WorkerPool::add_operations_to_queue(std::shared_ptr<Operation> prev_op, const Input &input) {
    if (prev_op == nullptr) {
        int all_ops = static_cast<int>(AllOperations::Op_Count);
        for (int i = 0; i < all_ops; i++) {
            auto op = create_operation_from_type(AllOperations(i), input);
            if (op) {
                add_job(op);
            }
        }
    } else {
        int all_ops = static_cast<int>(AllOperations::Op_Count);
        for (int i = 0; i < all_ops; i++) {
            auto op = create_operation_from_type_if_not_disallow(AllOperations(i), input, prev_op);
            if (op) {
                add_job(op);
            }
        }
    }
}

std::shared_ptr<Operation> WorkerPool::create_operation_from_type(AllOperations type, const Input &input) {
    switch (type) { ALL_OPS(CREATE_OP_SWITCH) }
    return nullptr;
}

std::shared_ptr<Operation> WorkerPool::create_operation_from_type_if_not_disallow(AllOperations type, const Input &input, std::shared_ptr<Operation> prev_op) {
    auto op = create_operation_from_type(type, input);
    if (op) {
        if (op->disallow_after(prev_op->type())) {
            return nullptr;
        }
        return op;
    }
    return nullptr;
}

void WorkerPool::add_job(std::shared_ptr<Operation> operation) {
    std::lock_guard<std::mutex> lock(m_jobs_mutex);
    m_jobs.push_back(operation);

    Terminal::print_pool_message(std::string("[+] Add job: \"") + AllOperations_to_string(operation->type()) + "\"");
}

void WorkerPool::mark_as_complete(std::shared_ptr<Operation> operation) {
    operation->setCompleted();

    const auto &output = operation->getOutput();

    if (output.is_valid()) {
        if (m_config.get_format().matches_format(output)) {
            Terminal::print_log_message("[+] Found valid flag: \"" + m_config.get_format().match(output) + "\"", Color::Green);

            stop();
        } else {
            add_operations_to_queue(operation, Input(output.get_output()));
        }
    } else {
        // invalid
        Terminal::print_pool_message(std::string("[-] Completed job: \"") + AllOperations_to_string(operation->type()) + "\" (invalid)");
    }
}

void WorkerPool::stop() {
    std::lock_guard<std::mutex> lock(m_jobs_mutex);

    if (is_stopped)
        return;

    for (auto &job : m_jobs) {
        job->setCompleted();
    }

    is_stopped = true;
    Terminal::print_log_message("[!] Stopped worker pool");
    Terminal::print_log_message("[!] Had a total of " + std::to_string(m_jobs.size()) + " jobs");
    for (int i = 0; i < m_assigned_jobs.size(); i++) {
        m_assigned_jobs[i] = nullptr;
        m_completed_jobs[i] = nullptr;
    }

    Terminal::print_all();  // one final print.

    for (auto &worker : m_workers) {
        worker->should_stop = true;
        while (!worker->has_stopped) {
        }
    }
}
}  // namespace ctf