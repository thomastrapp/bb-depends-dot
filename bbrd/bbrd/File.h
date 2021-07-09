// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include <string>
#include <stdexcept>


namespace bbrd {


/// Custom exception type for file errors.
class FileError : public std::runtime_error
{
public:
  explicit FileError(const std::string& msg) noexcept;
};


/// Read file at path to buffer. Throws FileError on failure.
/// Can read regular files as well as named pipes.
std::string ReadFileOrThrow(const std::string& path);


} // namespace bbtd

