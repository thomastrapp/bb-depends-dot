// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#include "bbrd/Dependencies.h"

#include <array>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>


namespace bbrd {


namespace ragel {
  %%{
    machine dot;
    include "dot-machine.rl";
    write data;
  }%%
} // namespace ragel


void Dependencies::extract_from_dot(const std::string& buffer)
{
  using namespace ragel;

  const char * p = buffer.data();
  const char * pe = buffer.data() + buffer.size();
  const char * eof = pe;
  int cs = 0;

  const char * recipe_name_begin = nullptr;
  std::string_view recipe_name;
  std::array<std::string_view, 2> stack;

  auto recipe_name_start = [&p, &recipe_name_begin](){
    recipe_name_begin = p;
  };

  auto recipe_name_stop = [&p, &recipe_name_begin, &recipe_name](){
    assert(recipe_name_begin);
    assert(p);
    if( !recipe_name_begin || !p )
      return;

    recipe_name = std::string_view(
      recipe_name_begin,
      static_cast<std::string_view::size_type>(
        std::distance(recipe_name_begin, p)
      )
    );
  };

  auto push_recipe = [&stack, &recipe_name](std::size_t i){
    stack.at(i) = recipe_name;
  };

  auto push_dependency = [&stack, this](){
    if( stack.at(0).compare(stack.at(1)) != 0 )
      this->add_dependency(stack.at(0), stack.at(1));
  };

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunreachable-code-break"
#endif
  %%{
    machine dot;
    include "dot-machine.rl";
    write init;
    write exec;
  }%%
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif
}

void Dependencies::add_dependency(std::string_view to, std::string_view from)
{
  this->dependencies_.push_back({
    this->get_or_create_id(to),
    this->get_or_create_id(from)
  });
}

Dependencies::Id Dependencies::get_or_create_id(std::string_view recipe)
{
  auto it = this->recipes_by_string_.find(recipe);
  if( it == this->recipes_by_string_.end() )
  {
    auto ret = this->recipes_by_string_.insert({recipe, this->next_id_});
    if( !ret.second )
      throw std::runtime_error("map insert failed");

    this->recipes_by_id_.push_back(recipe);
    this->next_id_++;
    return (ret.first)->second;
  }

  return it->second;
}


} // namespace bbrd

