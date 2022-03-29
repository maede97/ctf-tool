#pragma once

#include <memory>
#include <atomic>

namespace ctf
{
    class WorkerPool; // forward declaration
    /**
     * @brief A worker runs in a separate thread and executes the operations.
     */
    class Worker
    {
    public:
        Worker(int id, std::shared_ptr<WorkerPool> pool);

        void run();

        // Controlled by the pool
        std::atomic_bool should_stop;
        std::atomic_bool has_stopped;

    private:
        std::shared_ptr<WorkerPool> m_workerpool;
        int m_id;
    };
}