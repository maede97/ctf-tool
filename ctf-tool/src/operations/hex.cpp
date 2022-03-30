#include <ctf-tool/operations/hex.h>

#include <iomanip>
#include <sstream>

namespace ctf {
OpCls_FromHex::OpCls_FromHex(const Input& input, const Key& key) : Operation(AllOperations::Op_FromHex, input, key) {}

void OpCls_FromHex::run() {
    // convert input from hex to ascii
    std::string data = m_input.get_input();
    std::stringstream ss;
    for (int i = 0; i < data.size(); i += 2) {
        std::string sub = data.substr(i, 2);
        // check if both characters are valid hex characters
        if (std::isxdigit(sub[0]) && std::isxdigit(sub[1])) {
            ss << static_cast<char>(std::stoi(sub, nullptr, 16));
        } else {
            // found wrong hex character, aborting
            return;
        }
    }
    m_output = Output(ss.str(), true);
}

OpCls_ToHex::OpCls_ToHex(const Input& input, const Key& key) : Operation(AllOperations::Op_ToHex, input, key) {}

void OpCls_ToHex::run() {
    // convert input to hex
    std::string data = m_input.get_input();
    std::stringstream ss;
    for (char c : data) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }

    // set output
    m_output = Output(ss.str(), true);
}
}  // namespace ctf
