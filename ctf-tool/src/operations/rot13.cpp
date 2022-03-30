#include <ctf-tool/operations/rot13.h>

namespace ctf {
OpCls_Rot13::OpCls_Rot13(const Input &input, const Key &key, std::shared_ptr<Operation> previous_operation) : Operation(AllOperations::Op_Rot13, input, key, previous_operation) {}

void OpCls_Rot13::run() {
    std::string out;

    for(auto c : m_input.get_input()) {
        if(c >= 'a' && c <= 'z') {
            out += (c - 'a' + 13) % 26 + 'a';
        } else if(c >= 'A' && c <= 'Z') {
            out += (c - 'A' + 13) % 26 + 'A';
        } else {
            return;
        }
    }

    m_output = Output(out, true);
}

bool OpCls_Rot13::disallow_after(AllOperations type_prev) const {
    return type_prev == AllOperations::Op_Rot13;  // Rot13 after Rot13
}
}  // namespace ctf