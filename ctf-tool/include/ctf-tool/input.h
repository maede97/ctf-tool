#pragma once

#include <string>

namespace ctf {
class Input {
public:
    Input(const std::string &input);

    std::string get_input() const;

private:
    std::string m_input;
};
}  // namespace ctf