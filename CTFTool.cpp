#include <ctf-tool/include.h>

int main(int argc, char const *argv[])
{
    using namespace ctf;

    FlagFormat format("ctf{.*}");

    std::shared_ptr<WorkerPool> pool = WorkerPool::get_instance(format);

    Input my_input = Input("131515080f170a1c0d"); // FromHex --> XOR (password) --> ctf{xxxx}

    pool->set_key(Key("password"));
    pool->solve(my_input, 4);

    return 0;
}
