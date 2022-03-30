#include <ctf-tool/config.h>
#include <ctf-tool/terminal.h>

#include <fstream>

namespace ctf {

Config::Config() {
    m_key = Key("");
    m_format = FlagFormat("");
    m_input = Input("");
    m_num_workers = 0;
}

Config::~Config() {}

void Config::read_config(const std::string& config_file) {
    // check if the file exists, if not, create it
    if (!std::ifstream(config_file).good()) {
        create_config(config_file);
    }

    std::ifstream file(config_file);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("key") != std::string::npos) {
            m_key = Key(line.substr(line.find(":") + 2));
        } else if (line.find("format") != std::string::npos) {
            m_format = FlagFormat(line.substr(line.find(":") + 2));
        } else if (line.find("input") != std::string::npos) {
            m_input = Input(line.substr(line.find(":") + 2));
        } else if (line.find("num_workers") != std::string::npos) {
            m_num_workers = std::stoi(line.substr(line.find(":") + 2));
        }
    }

    Terminal::print_log_message("[!] Read config file: " + config_file);
}

void Config::create_config(const std::string& config_file) {
    std::ofstream file(config_file);
    file << "key: password\n";
    file << "format: ctf{xxxx}\n";
    file << "input: 131515080f170a1c0d\n";
    file << "num_workers: 4\n";

    Terminal::print_log_message("[!] Created config file: " + config_file);
}

void Config::set_key(const Key& key) {
    m_key = key;
}

void Config::set_format(const FlagFormat& format) {
    m_format = format;
}

void Config::set_input(const Input& input) {
    m_input = input;
}

void Config::set_num_workers(int num_workers) {
    m_num_workers = num_workers;
}

const Key& Config::get_key() const {
    return m_key;
}

const FlagFormat& Config::get_format() const {
    return m_format;
}

const Input& Config::get_input() const {
    return m_input;
}

int Config::get_num_workers() const {
    return m_num_workers;
}

}  // namespace ctf