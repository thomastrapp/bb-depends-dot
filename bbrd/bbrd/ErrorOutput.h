// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include <iostream>
#include <string>
#ifndef _WIN32
#include <unistd.h>
#endif


namespace bbrd {


/// Helper class to output errors. Use ANSI escape codes to highlight source
/// and title of an error messsage if STDERR is a terminal.
class ErrorOutput
{
public:
  explicit ErrorOutput(const char * error_source,
                       std::ostream& out = std::cerr) noexcept;

  void print(const std::string& title, const std::string& contents) const;

private:
#ifndef _WIN32
  // Change foreground color to bold red
  static constexpr const char * esc_red_ = "\033[1;31m";
  static constexpr const char * esc_reset_ = "\033[0m";
#endif

  std::ostream& out_;
  const char * source_;
#ifndef _WIN32
  bool isatty_;
#endif
};


} // namespace bbrd

