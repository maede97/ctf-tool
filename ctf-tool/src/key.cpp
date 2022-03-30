#include <ctf-tool/key.h>

namespace ctf {

Key::Key(const std::string &key) : m_key(key) {}

std::string Key::get_key() const {
    return m_key;
}

}  // namespace ctf