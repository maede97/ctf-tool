#pragma once

#include <string>
#include <vector>

namespace ctf {

enum class Color { Default, Green, Red, Blue };

// static class that handles terminal output
class Terminal {
public:
    static int KEEP_NUM_LOG_ENTRIES;  // default 10
    static void print_log_message(const std::string& message, Color color = Color::Default);
    static void print_worker_message(const std::string& message, Color color = Color::Default);
    static void print_pool_message(const std::string& message, Color color = Color::Default);

    static void print_all();

private:
    static std::vector<std::string>& log();
    static std::vector<std::string>& worker();
    static std::vector<std::string>& pool();
};

}  // namespace ctf