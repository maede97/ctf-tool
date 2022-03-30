#include <ctf-tool/input.h>

namespace ctf {
Input::Input(const std::string &input) : m_input(input) {}

std::string Input::get_input() const {
    return m_input;
}
}  // namespace ctf