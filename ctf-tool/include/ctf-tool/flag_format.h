#pragma once

#include <ctf-tool/output.h>
#include <string>
#include <regex>

namespace ctf
{

    class FlagFormat
    {
    public:
        FlagFormat(const std::string &format);

        bool matches_format(const Output &op_output) const;
        std::string match(const Output &output) const;

    private:
        std::regex m_regex;
    };
}