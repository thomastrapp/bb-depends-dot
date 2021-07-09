// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include <iostream>
#include <boost/program_options.hpp>


namespace bbrd {


class ProgramOptions
{
public:
  ProgramOptions();

  void store_and_validate_or_throw(int argc, const char * argv[]);
  bool contains(const char * key) const;
  std::string get(const char * key) const;
  void print(const char * program_name, std::ostream& out = std::cout) const;

private:
  boost::program_options::options_description desc_;
  boost::program_options::variables_map vm_;
};


} // namespace bbrd

