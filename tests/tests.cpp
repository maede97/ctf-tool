#include <ctf-tool/operations/operations.h>
#include <iostream>

void test_xor()
{
    using namespace ctf;
    // input: 131515080f170a1c0d --> FromHex --> XOR (password) --> ctf{xxxx}
    // FromHex
    OpCls_FromHex op_from_hex(Input("131515080f170a1c0d"));
    op_from_hex.run();
    // XOR
    OpCls_XOR xor_op(op_from_hex.getOutput().get_output(), Key("password"));
    xor_op.run();

    std::cout << "[Test] XOR got " << xor_op.getOutput().get_output() << std::endl;
}

int main(int argc, char const *argv[])
{
    test_xor();
    return 0;
}
