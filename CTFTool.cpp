#include <ctf-tool/include.h>

int main(int argc, char const *argv[]) {
    using namespace ctf;

    Config config;
    config.read_config("../ctf-tool.ini");

    std::shared_ptr<WorkerPool> pool = WorkerPool::get_instance(config);

    pool->solve();

    return 0;
}
