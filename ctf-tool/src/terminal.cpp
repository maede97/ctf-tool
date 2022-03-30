#include <ctf-tool/terminal.h>
#include <stdio.h>  // for printf

namespace ctf {

int Terminal::KEEP_NUM_LOG_ENTRIES = 7;

std::string color_to_string(Color color) {
    switch (color) {
        case Color::Default:
            return "";
        case Color::Green:
            return "\033[32m";
        case Color::Red:
            return "\033[31m";
        case Color::Blue:
            return "\033[34m";
        default:
            return "";
    }
}

std::string add_color(const std::string& message, Color color) {
    return color_to_string(color) + message + "\033[0m";
}

void Terminal::print_log_message(const std::string& message, Color color) {
    log().push_back(add_color(message, color));
    if (log().size() > Terminal::KEEP_NUM_LOG_ENTRIES) {
        log().erase(log().begin());
    }
}

void Terminal::print_worker_message(const std::string& message, Color color) {
    worker().push_back(add_color(message, color));
    if (worker().size() > Terminal::KEEP_NUM_LOG_ENTRIES) {
        worker().erase(worker().begin());
    }
}

void Terminal::print_pool_message(const std::string& message, Color color) {
    pool().push_back(add_color(message, color));
    if (pool().size() > Terminal::KEEP_NUM_LOG_ENTRIES) {
        pool().erase(pool().begin());
    }
}

void Terminal::print_all() {
    // first, clear the screen
    printf("\033[2J");
    printf("\033[0;0H");

    // print a custom header with "CTF-Tool" in ASCII art
    // change color to gold
    printf("\n");
    printf("\033[38;5;214m");
    printf("   ******  ********** ********       **********                    **\n");
    printf("  **////**/////**/// /**/////       /////**///                    /**\n");
    printf(" **    //     /**    /**                /**      ******   ******  /**\n");
    printf("/**           /**    /*******  *****    /**     **////** **////** /**\n");
    printf("/**           /**    /**////  /////     /**    /**   /**/**   /** /**\n");
    printf("//**    **    /**    /**                /**    /**   /**/**   /** /**\n");
    printf(" //******     /**    /**                /**    //****** //******  ***\n");
    printf("  //////      //     //                 //      //////   //////  /// \n");
    printf("\033[0m");

    printf("\n");
    printf("By maede97\n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("\n");

    // print the log
    for (const auto& message : log()) {
        printf("%s\n", message.c_str());
    }
    for (int i = log().size(); i < Terminal::KEEP_NUM_LOG_ENTRIES; i++) {
        printf("\n");
    }

    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("\n");

    // print the worker messages
    for (const auto& message : worker()) {
        printf("%s\n", message.c_str());
    }
    for (int i = worker().size(); i < Terminal::KEEP_NUM_LOG_ENTRIES; i++) {
        printf("\n");
    }

    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("\n");

    // print the pool messages
    for (const auto& message : pool()) {
        printf("%s\n", message.c_str());
    }
    for (int i = pool().size(); i < Terminal::KEEP_NUM_LOG_ENTRIES; i++) {
        printf("\n");
    }

    printf("\n");
    printf("---------------------------------------------------------------------\n");
}

std::vector<std::string>& Terminal::log() {
    static std::vector<std::string> log;
    return log;
}

std::vector<std::string>& Terminal::worker() {
    static std::vector<std::string> worker;
    return worker;
}

std::vector<std::string>& Terminal::pool() {
    static std::vector<std::string> pool;
    return pool;
}

}  // namespace ctf