// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include <iostream>


namespace bbrd {


/// Major version numer.
extern const int version_major;

/// Minor version number.
extern const int version_minor;


/// Print version to out.
void PrintVersion(std::ostream& out = std::cout);


} // namespace bbrd

