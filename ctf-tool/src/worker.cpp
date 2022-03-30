#include <ctf-tool/worker.h>
#include <ctf-tool/workerpool.h>

#include <iostream>

namespace ctf {
Worker::Worker(int id, std::shared_ptr<WorkerPool> pool) : m_id(id), m_workerpool(pool) {
    should_stop = false;
    has_stopped = false;
}

void Worker::run() {
    while (!should_stop) {
        auto operation = m_workerpool->m_assigned_jobs[m_id];
        if (operation) {
            operation->run();
            m_workerpool->m_completed_jobs[m_id] = operation;
        }
    }

    has_stopped = true;
}
}  // namespace ctf