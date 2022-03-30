#include <ctf-tool/operations/pass_through.h>

namespace ctf {
OpCls_PassThrough::OpCls_PassThrough(const Input &input, const Key &key, std::shared_ptr<Operation> previous_operation) : Operation(AllOperations::Op_PassThrough, input, key, previous_operation) {}

void OpCls_PassThrough::run() {
    m_output = Output(m_input.get_input(), true);  // pass through is always valid
}

bool OpCls_PassThrough::disallow_after(AllOperations type_prev) const {
    return type_prev == AllOperations::Op_PassThrough;  // pass through can't be followed by pass through
}
}  // namespace ctf