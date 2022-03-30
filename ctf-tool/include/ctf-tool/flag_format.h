#pragma once

#include <ctf-tool/output.h>

#include <regex>
#include <string>

namespace ctf {

class FlagFormat {
public:
    FlagFormat() = default;
    FlagFormat(const std::string &format);

    bool matches_format(const Output &op_output) const;
    std::string match(const Output &output) const;

private:
    std::regex m_regex;
};
}  // namespace ctf