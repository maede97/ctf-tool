#pragma once

#include <ctf-tool/flag_format.h>
#include <ctf-tool/key.h>
#include <ctf-tool/operation.h>
#include <ctf-tool/worker.h>

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace ctf {
class WorkerPool : public std::enable_shared_from_this<WorkerPool> {
public:
    static std::shared_ptr<WorkerPool> get_instance(const FlagFormat &format);

    void set_key(const Key &key);

    void solve(const Input &input, int num_workers);

    ~WorkerPool();

private:
    WorkerPool(const FlagFormat &format);

    void add_job(std::shared_ptr<Operation> operation);

    void mark_as_complete(std::shared_ptr<Operation> operation);

    bool has_open_work() const;
    std::shared_ptr<Operation> get_next_job();

    friend class Worker;

protected:
    // Internal data for the workers
    std::vector<std::shared_ptr<Operation>> m_completed_jobs;
    std::vector<std::shared_ptr<Operation>> m_assigned_jobs;

private:
    void add_operations_to_queue(std::shared_ptr<Operation> prev_op, const Input &input);
    std::shared_ptr<Operation> create_operation_from_type(AllOperations type, const Input &input);
    std::shared_ptr<Operation> create_operation_from_type_if_not_disallow(AllOperations type, const Input &input, std::shared_ptr<Operation> prev_op);

    void stop();

private:
    std::vector<std::shared_ptr<Operation>> m_jobs;

    std::vector<std::shared_ptr<Worker>> m_workers;
    mutable std::mutex m_jobs_mutex;

    const FlagFormat &m_format;
    Key m_key;
    bool is_stopped = false;
};
}  // namespace ctf