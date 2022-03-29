#include <ctf-tool/operations/xor.h>

namespace ctf
{
    OpCls_XOR::OpCls_XOR(const Input &input, const Key &key) : Operation(AllOperations::Op_XOR, input, key)
    {
    }

    void OpCls_XOR::run()
    {
        // check if key exists
        if (m_key.get_key().size() == 0)
        {
            return;
        }

        std::string key = m_key.get_key();
        std::string input = m_input.get_input();

        // xor the input and key
        std::string out;
        for (size_t i = 0; i < input.size(); i++)
        {
            out.push_back(input[i] ^ key[i % key.size()]);
        }

        m_output = Output(out, true);
    }

    bool OpCls_XOR::disallow_after(AllOperations type_prev) const
    {
        return type_prev == AllOperations::Op_XOR; // XOR after XOR
    }
}