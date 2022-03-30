#include <ctf-tool/operations/base64.h>

#include <vector>

namespace ctf {
OpCls_Base64Encode::OpCls_Base64Encode(const Input& input, const Key& key, std::shared_ptr<Operation> previous_operation)
    : Operation(AllOperations::Op_Base64Encode, input, key, previous_operation) {}

void OpCls_Base64Encode::run() {
    std::string out;

    std::string in = m_input.get_input();
    typedef unsigned char uchar;
    unsigned int val = 0;
    int valb = -6;
    for (uchar c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');

    m_output = Output(out, true);
}

bool OpCls_Base64Encode::disallow_after(AllOperations prev_type) const {
    return prev_type == AllOperations::Op_Base64Decode;
}

OpCls_Base64Decode::OpCls_Base64Decode(const Input& input, const Key& key, std::shared_ptr<Operation> previous_operation)
    : Operation(AllOperations::Op_Base64Decode, input, key, previous_operation) {}

void OpCls_Base64Decode::run() {
    std::string out;
    std::string in = m_input.get_input();

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    typedef unsigned char uchar;
    unsigned int val = 0;
    int valb = -8;
    for (uchar c : in) {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    m_output = Output(out, true);
}

bool OpCls_Base64Decode::disallow_after(AllOperations prev_type) const {
    return prev_type == AllOperations::Op_Base64Encode;
}

}  // namespace ctf