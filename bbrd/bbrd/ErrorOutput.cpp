// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#include "bbrd/ErrorOutput.h"

#include <ostream>
#include <string>
#ifndef _WIN32
#include <unistd.h>
#endif


namespace bbrd {


ErrorOutput::ErrorOutput(const char * error_source, std::ostream& out) noexcept
: out_(out)                      // noexcept
, source_(error_source)          // noexcept
#ifndef _WIN32
, isatty_(isatty(STDERR_FILENO)) // noexcept
#endif
{
}

void ErrorOutput::print(const std::string& title,
                        const std::string& contents) const
{
  this->out_
#ifndef _WIN32
    << ( this->isatty_ ? ErrorOutput::esc_red_ : "" )
#endif
    << this->source_ << ": "
    << title << ": "
#ifndef _WIN32
    << ( this->isatty_ ? ErrorOutput::esc_reset_ : "" )
#endif
    << contents << "\n";
}


} // namespace bbrd

