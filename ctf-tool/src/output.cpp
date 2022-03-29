#include <ctf-tool/output.h>

namespace ctf
{
    Output::Output(const std::string &output, bool is_valid) : m_output(output), m_is_valid(is_valid) {}

    bool Output::is_valid() const { return m_is_valid; }
    std::string Output::get_output() const { return m_output; }
}