#include <ctf-tool/include.h>

int main(int argc, char const *argv[])
{
    using namespace ctf;

    FlagFormat format("ctf{.*}");

    std::shared_ptr<WorkerPool> pool = WorkerPool::get_instance(format);

    Input my_input = Input("546b64564d6c6c55556d744e656b3077576c52524d4535556133704e616c4a735458704a4d4539555458704f52316b77546b525a656b3136555442615647526f546d706a656b3136556d314f52464579546b526161513d3d");

    pool->solve(my_input, 4);

    return 0;
}
