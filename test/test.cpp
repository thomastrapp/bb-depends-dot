// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include <bbrd/Dependencies.h>
#include <bbrd/DependencyGraph.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <catch2/catch.hpp>


namespace {

  using DependencyTestData
    = std::unordered_map<std::string, std::vector<std::string>>;

  namespace simple_dot {
    const char * buffer = R"dot(
digraph depends {
  "boost" -> "libc"
  "image" -> "htmlext"
  "htmlext" -> "libhext"
  "htmlext" -> "boost-program-options"
  "libhext" -> "ragel"
  "libhext" -> "boost-regex"
  garbage input
  "nope"
  "boost-regex" -> "boost"
  "boost-program-options" -> "boost"
}
)dot";
    std::vector<const char *> distinct_recipes = {
      "boost",
      "boost-program-options",
      "boost-regex",
      "htmlext",
      "image",
      "libc",
      "libhext",
      "ragel",
    };

    DependencyTestData direct_dependencies = {
      {
        "htmlext", {
          "libhext",
          "boost-program-options",
        },
      },
      {
        "libc", {},
      },
      {
        "boost-regex", {
          "boost",
        },
      },
    };

    DependencyTestData direct_reverse_dependencies = {
      {
        "htmlext", {
          "image",
        },
      },
      {
        "libc", {
          "boost",
        },
      },
      {
        "boost-regex", {
          "libhext",
        },
      },
    };

    DependencyTestData transitive_dependencies = {
      {
        "htmlext", {
          "libhext",
          "boost-program-options",
          "ragel",
          "boost",
          "boost-regex",
          "libc",
        },
      },
      {
        "libc", {},
      },
      {
        "boost-regex", {
          "boost",
          "libc",
        },
      },
    };

    DependencyTestData transitive_reverse_dependencies = {
      {
        "htmlext", {
          "image",
        },
      },
      {
        "libc", {
          "boost",
          "boost-regex",
          "boost-program-options",
          "libhext",
          "htmlext",
          "image",
        },
      },
      {
        "boost-regex", {
          "libhext",
          "htmlext",
          "image",
        },
      },
    };


  } // namespace simple_dot


  template<typename F>
  std::vector<std::string> run_and_extract(
      F func, const std::string& recipe, bool reverse)
  {
    std::vector<std::string> dependencies;
    std::stringstream sstream;
    func(recipe, reverse, sstream);
    std::string line;
    while( std::getline(sstream, line) )
      dependencies.push_back(line);
    return dependencies;
  }

  template<typename F>
  void compare_test_data(
      F func, bool reverse, const DependencyTestData& test_data)
  {
    for( auto t : test_data )
    {
      INFO("Test data key " << t.first);
      auto recipe = t.first;
      auto expected_result = t.second;
      auto result = run_and_extract(func, recipe, reverse);
      std::sort(expected_result.begin(), expected_result.end());
      std::sort(result.begin(), result.end());
      REQUIRE( expected_result == result );
    }
  }


} // namespace


TEST_CASE("dependencies-from-dot")
{
  auto deps = bbrd::Dependencies(simple_dot::buffer);

  REQUIRE( deps.distinct_recipe_count() ==
           simple_dot::distinct_recipes.size() );
  auto recipe_count_from_names =
    static_cast<std::size_t>(
      std::distance(deps.names_begin(), deps.names_end())
    );
  REQUIRE( deps.distinct_recipe_count() == recipe_count_from_names );

  for( auto recipe : simple_dot::distinct_recipes )
    REQUIRE( deps.get_recipe_id(recipe).has_value() );
}

TEST_CASE("dependencies-empty")
{
  bbrd::Dependencies deps("");
  REQUIRE( deps.distinct_recipe_count() == 0 );
  REQUIRE_THROWS( deps.get_recipe_name(0) );
  REQUIRE_THROWS( deps.get_recipe_name(1) );
  REQUIRE( deps.get_recipe_id("nope").has_value() == false );
  REQUIRE( deps.get_recipe_id("").has_value() == false );
}

TEST_CASE("dependencies")
{
  bbrd::Dependencies deps(R"dot(
"left" -> "right"
)dot");
  REQUIRE( deps.distinct_recipe_count() == 2 );
  auto recipe_count_from_names =
    static_cast<std::size_t>(
      std::distance(deps.names_begin(), deps.names_end())
    );
  REQUIRE( deps.distinct_recipe_count() == recipe_count_from_names );
  REQUIRE( deps.get_recipe_name(0).compare("left") == 0 );
  REQUIRE( deps.get_recipe_name(1).compare("right") == 0 );
  auto dependency_count =
      static_cast<std::size_t>(
        std::distance(deps.begin(), deps.end())
      );
  REQUIRE( dependency_count == 1 );
  REQUIRE( deps.get_recipe_id("left").has_value() );
  REQUIRE( deps.get_recipe_id("right").has_value() );
}

TEST_CASE("dependency-graph-empty")
{
  bbrd::DependencyGraph graph(bbrd::Dependencies(""));
  std::stringstream sstream;
  REQUIRE_THROWS( graph.list_recipe_depends("", true, sstream) );
  REQUIRE_THROWS( graph.list_adjacent_recipes("", true, sstream) );
  REQUIRE_THROWS( graph.list_recipe_depends("nope", true, sstream) );
  REQUIRE_THROWS( graph.list_adjacent_recipes("nope", true, sstream) );
}

TEST_CASE("dependency-graph-test-data")
{
  auto graph = bbrd::DependencyGraph(
      bbrd::Dependencies(simple_dot::buffer));

  using F = std::function<void(std::string_view, bool, std::ostream&)>;
  F list_adjacent_recipes = std::bind(
      &bbrd::DependencyGraph::list_adjacent_recipes,
      &graph,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3);
  F list_recipe_depends = std::bind(
      &bbrd::DependencyGraph::list_recipe_depends,
      &graph,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3);

  using T = std::tuple<F, bool, const DependencyTestData&>;
  std::vector<T> test_data = {
    {list_adjacent_recipes, false, simple_dot::direct_dependencies},
    {list_adjacent_recipes, true, simple_dot::direct_reverse_dependencies},
    {list_recipe_depends, false, simple_dot::transitive_dependencies},
    {list_recipe_depends, true, simple_dot::transitive_reverse_dependencies},
  };

  for(std::size_t i = 0; i != test_data.size(); ++i)
  {
    INFO("Test data index " << i);
    compare_test_data(std::get<0>(test_data.at(i)),
                      std::get<1>(test_data.at(i)),
                      std::get<2>(test_data.at(i)));
  }
}

