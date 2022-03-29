#include <ctf-tool/operations/pass_through.h>

namespace ctf
{
    OpCls_PassThrough::OpCls_PassThrough(const Input &input, const Key &key) : Operation(AllOperations::Op_PassThrough, input, key)
    {
    }

    void OpCls_PassThrough::run()
    {
        m_output = Output(m_input.get_input(), true); // pass through is always valid
    }

    bool OpCls_PassThrough::disallow_after(AllOperations type_prev) const
    {
        return type_prev == AllOperations::Op_PassThrough; // pass through can't be followed by pass through
    }
}