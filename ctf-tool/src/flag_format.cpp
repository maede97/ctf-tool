#include <ctf-tool/flag_format.h>

#include <sstream>

namespace ctf {

FlagFormat::FlagFormat(const std::string &format) {
    std::stringstream regex_builder;
    regex_builder << "(";  // group matching

    for (char c : format) {
        if (c == '{') {
            regex_builder << "\\{";
        } else if (c == '}') {
            regex_builder << "\\}";
        } else {
            regex_builder << c;
        }
    }
    regex_builder << ")";  // group matching

    m_regex = std::regex(regex_builder.str());
}

bool FlagFormat::matches_format(const Output &op_output) const {
    return std::regex_search(op_output.get_output(), m_regex);
}

std::string FlagFormat::match(const Output &output) const {
    std::smatch match;
    std::string output_str = output.get_output();
    bool has = std::regex_search(output_str, match, m_regex);
    if (has) {
        return match[1];
    } else {
        return "<Nothing extracted>";
    }
}

}  // namespace ctf