#pragma once

#include <iostream>

namespace heroes {
#define LOG(...) heroes::Logger::message(__FILE__, __LINE__, __VA_ARGS__)
class Logger {
public:
  template <typename... Args>
  static void message(const char *filename, std::size_t line,
                      const Args &... args) {
    std::cerr << "In file"
              << " " << filename << " "
              << "On line num"
              << " " << line << '\n';
    std::cerr << "   "
              << "Message:"
              << " ";
    (std::cerr << ... << args) << '\n';
  }
};
} // namespace heroes
