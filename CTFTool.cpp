#include <ctf-tool/include.h>

int main(int argc, char const *argv[])
{
    using namespace ctf;

    FlagFormat format("(ctf\\{.*\\})");

    std::shared_ptr<WorkerPool> pool = WorkerPool::get_instance(format);

    Input my_input = Input("WTNSbWUzaDRlSGg5Q2c9PQo="); // double base64 encoded flag.

    pool->solve(my_input, 4);

    return 0;
}
