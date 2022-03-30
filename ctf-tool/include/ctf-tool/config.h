#pragma once

#include <ctf-tool/flag_format.h>
#include <ctf-tool/input.h>
#include <ctf-tool/key.h>

namespace ctf {

class Config {
public:
    Config();
    ~Config();

    void read_config(const std::string &config_file);
    void create_config(const std::string &config_file);

    void set_key(const Key &key);
    void set_format(const FlagFormat &format);
    void set_input(const Input &input);
    void set_num_workers(int num_workers);

    const Key &get_key() const;
    const FlagFormat &get_format() const;
    const Input &get_input() const;
    int get_num_workers() const;

private:
    Key m_key;
    FlagFormat m_format;
    Input m_input;
    int m_num_workers;
};

}  // namespace ctf