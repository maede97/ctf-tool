#pragma once
#include <string>
namespace ctf
{
    class Output
    {
    public:
        Output() = default;
        Output(const std::string &output, bool is_valid);

        bool is_valid() const;
        std::string get_output() const;

    private:
        std::string m_output;
        bool m_is_valid = false;
    };

} // namespace ctf
