#pragma once

#include <string>

namespace ctf {
class Key {
public:
    Key() = default;
    Key(const std::string &key);

    std::string get_key() const;

private:
    std::string m_key;
};
}  // namespace ctf