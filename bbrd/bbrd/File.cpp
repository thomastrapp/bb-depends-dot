// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#include "bbrd/File.h"

#include <fstream>
#include <sstream>
// strerror_r, strerror_s are not part of the C++ stdlib
#include <string.h>


namespace {


/// Wrapper for strerror_r (GNU) and strerror_s (MSVC).
std::string StrError(int errnum)
{
  // Buffer size: There may be locales where error messages get huge in terms
  // of bytes and performance is no issue in my use case.
  std::string buffer(1024, '\0');
#ifdef _WIN32
  // strerror_s returns int 0 if it successfully populated the buffer.
  if( strerror_s(&buffer[0], buffer.size(), errnum) )
    return "error message unavailable";
  else
    return buffer;
#elif defined(_GNU_SOURCE)
  // strerror_r may use the buffer, but doesn't need to. It may return a
  // pointer to a string that is already available.
  return std::string(strerror_r(errnum, &buffer[0], buffer.size()));
#elif __APPLE__
  // strerror_r returns int 0 if it successfully populated the buffer.
  if( strerror_r(errnum, &buffer[0], buffer.size()) )
    return "error message unavailable";
  else
    return buffer;
#else
  #error "Unsupported platform. Please raise an issue on github."
#endif
}


} // namespace


namespace bbrd {


FileError::FileError(const std::string& msg) noexcept
: std::runtime_error(msg)  // noexcept
{
}


std::string ReadFileOrThrow(const std::string& path)
{
  std::ifstream file(path, std::ios::in | std::ios::binary);

  if( file.fail() )
  {
    if( path == "-" )
      return ReadFileOrThrow("/dev/stdin");
    else
      throw FileError(
        "cannot access '" + path + "': " + StrError(errno)
      );
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  if( file.fail() )
    throw FileError(
      "cannot read '" + path + "': " + StrError(errno)
    );

  return buffer.str();
}


} // namespace bbrd

