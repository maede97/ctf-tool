#include <ctf-tool/workerpool.h>
#include <ctf-tool/operations/operations.h>
#include <functional>
#include <iostream>

namespace ctf
{
#define CREATE_OP_SWITCH(op)     \
    case AllOperations::Op_##op: \
        return std::make_shared<OpCls_##op>(input, m_key);

    std::shared_ptr<WorkerPool> WorkerPool::get_instance(const FlagFormat &format)
    {
        return std::shared_ptr<WorkerPool>(new WorkerPool(format));
    }

    WorkerPool::WorkerPool(const FlagFormat &format) : m_format(format)
    {
    }

    WorkerPool::~WorkerPool()
    {
        stop();
    }

    void WorkerPool::set_key(const Key &key)
    {
        m_key = key;
    }

    void WorkerPool::solve(const Input &input, int num_workers)
    {
        for (int i = 0; i < num_workers; i++)
        {
            m_workers.push_back(std::make_shared<Worker>(i, shared_from_this()));
        }
        m_completed_jobs.resize(num_workers, nullptr); // each worker has a spot
        m_assigned_jobs.resize(num_workers, nullptr);

        // first: add all operations to the queue
        add_operations_to_queue(nullptr, input);

        // second: create all threads
        for (auto &worker : m_workers)
        {
            // create a thread with the run function
            std::thread thread(std::bind(&Worker::run, worker));
            thread.detach();
        }

        while (has_open_work())
        {
            // check a thread for "open-to-work"
            for (int i = 0; i < num_workers; i++)
            {
                if (m_assigned_jobs[i] == nullptr)
                {
                    auto operation = get_next_job();
                    if (operation)
                    {
                        operation->setAssigned();
                        // assign the job to the worker
                        m_assigned_jobs[i] = operation;
                        // std::cout << "[" << i << "] Worker got job: \"" << AllOperations_to_string(operation->type()) << "\"" << std::endl;
                    }
                }

                if (m_assigned_jobs[i] == m_completed_jobs[i] && m_completed_jobs[i] != nullptr)
                {
                    // the job is complete
                    // std::cout << "[" << i << "] Worker completed job \"" << AllOperations_to_string(m_completed_jobs[i]->type()) << "\"" << std::endl;
                    m_assigned_jobs[i] = nullptr;
                    mark_as_complete(m_completed_jobs[i]);
                    m_completed_jobs[i] = nullptr;

                    if (is_stopped)
                        return;
                }
            }
        }

        stop();
    }

    std::shared_ptr<Operation> WorkerPool::get_next_job()
    {
        std::lock_guard<std::mutex> lock(m_jobs_mutex);
        for (auto &job : m_jobs)
        {
            if (job->status() == OperationStatus::NotStarted)
            {
                return job;
            }
        }
        return nullptr;
    }

    bool WorkerPool::has_open_work() const
    {
        std::lock_guard<std::mutex> lock(m_jobs_mutex);

        // check if any job is still in running / waiting state
        for (auto &job : m_jobs)
        {
            if (job->status() != OperationStatus::Finished)
            {
                return true;
            }
        }

        for (int i = 0; i < m_assigned_jobs.size(); i++)
        {
            if (m_assigned_jobs[i] != nullptr)
            {
                return true;
            }
        }

        for (int i = 0; i < m_completed_jobs.size(); i++)
        {
            if (m_completed_jobs[i] != nullptr)
            {
                return true;
            }
        }

        return false;
    }

    void WorkerPool::add_operations_to_queue(std::shared_ptr<Operation> prev_op, const Input &input)
    {
        if (prev_op == nullptr)
        {
            int all_ops = static_cast<int>(AllOperations::Op_Count);
            for (int i = 0; i < all_ops; i++)
            {
                auto op = create_operation_from_type(AllOperations(i), input);
                if (op)
                {
                    add_job(op);
                }
            }
        }
        else
        {
            int all_ops = static_cast<int>(AllOperations::Op_Count);
            for (int i = 0; i < all_ops; i++)
            {
                auto op = create_operation_from_type_if_not_disallow(AllOperations(i), input, prev_op);
                if (op)
                {
                    add_job(op);
                }
            }
        }
    }

    std::shared_ptr<Operation> WorkerPool::create_operation_from_type(AllOperations type, const Input &input)
    {
        switch (type)
        {
            ALL_OPS(CREATE_OP_SWITCH)
        }
        return nullptr;
    }

    std::shared_ptr<Operation> WorkerPool::create_operation_from_type_if_not_disallow(AllOperations type, const Input &input, std::shared_ptr<Operation> prev_op)
    {
        auto op = create_operation_from_type(type, input);
        if (op)
        {
            if (op->disallow_after(prev_op->type()))
            {
                return nullptr;
            }
            return op;
        }
        return nullptr;
    }

    void WorkerPool::add_job(std::shared_ptr<Operation> operation)
    {
        std::lock_guard<std::mutex> lock(m_jobs_mutex);
        m_jobs.push_back(operation);

        // std::cout << "[+] Add job \"" << AllOperations_to_string(operation->type()) << "\"" << std::endl;
    }

    void WorkerPool::mark_as_complete(std::shared_ptr<Operation> operation)
    {
        operation->setCompleted();

        const auto &output = operation->getOutput();

        if (output.is_valid())
        {
            if (m_format.matches_format(output))
            {
                std::cout << "[!] Found valid flag: " << m_format.match(output) << std::endl;

                stop();
            }
            else
            {
                add_operations_to_queue(operation, Input(output.get_output()));
            }
        }
        else
        {
            // invalid
            // std::cout << "[-] Completed job \"" << AllOperations_to_string(operation->type()) << "\" (invalid)" << std::endl;
        }
    }

    void WorkerPool::stop()
    {
        std::lock_guard<std::mutex> lock(m_jobs_mutex);

        if (is_stopped)
            return;

        for (auto &job : m_jobs)
        {
            job->setCompleted();
        }

        is_stopped = true;
        std::cout << "[!] Shutting down..." << std::endl;
        std::cout << "[!] Had a total of " << m_jobs.size() << " jobs" << std::endl;
        for (int i = 0; i < m_assigned_jobs.size(); i++)
        {
            m_assigned_jobs[i] = nullptr;
            m_completed_jobs[i] = nullptr;
        }

        for (auto &worker : m_workers)
        {
            worker->should_stop = true;
            while (!worker->has_stopped)
            {
            }
        }
    }
}