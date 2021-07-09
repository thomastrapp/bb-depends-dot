// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#include "bbrd/Dependencies.h"
#include "bbrd/DependencyGraph.h"
#include "bbrd/ErrorOutput.h"
#include "bbrd/File.h"
#include "bbrd/ProgramOptions.h"
#include "bbrd/Version.h"

#include <cstdlib>
#include <ios>
#include <iostream>


int main(int argc, const char * argv[])
{
  std::ios::sync_with_stdio(false);
  bbrd::ProgramOptions po;
  bbrd::ErrorOutput errout(argv[0]);

  try
  {
    po.store_and_validate_or_throw(argc, argv);

    if( po.contains("help") )
    {
      po.print(argv[0], std::cout);
      return EXIT_SUCCESS;
    }

    if( po.contains("version") )
    {
      bbrd::PrintVersion(std::cout);
      return EXIT_SUCCESS;
    }

    auto input_file = po.get("task-depends-dot");
    bbrd::DependencyGraph graph(
        bbrd::Dependencies(
          bbrd::ReadFileOrThrow(input_file)));

    if( po.contains("recipe") )
    {
      bool reverse = po.contains("rdepends");
      std::string recipe = po.get("recipe");

      if( po.contains("transitive") )
        graph.list_recipe_depends(
            recipe,
            reverse,
            std::cout);
      else
        graph.list_adjacent_recipes(
            recipe,
            reverse,
            std::cout);
    }
    else
    {
      graph.list(std::cout);
    }
  }
  catch( const bbrd::FileError& e )
  {
    errout.print("Error", e.what());
    return EXIT_FAILURE;
  }
  catch( const boost::program_options::error& e )
  {
    errout.print("Argument error", e.what());
    if( argc < 2 )
    {
      std::cerr << "\n";
      po.print(argv[0], std::cerr);
    }
    return EXIT_FAILURE;
  }
  catch( const std::exception& e )
  {
    errout.print("Error", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

