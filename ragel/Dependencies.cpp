
#line 1 "Dependencies.cpp.rl"
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
  
#line 22 "Dependencies.cpp"
static const char _dot_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 6, 2, 4, 5
};

static const char _dot_key_offsets[] = {
	0, 0, 4, 11, 20, 22, 24, 25, 
	27, 34, 43, 44, 45, 46, 50, 54
};

static const char _dot_trans_keys[] = {
	32, 34, 9, 13, 45, 48, 57, 65, 
	90, 97, 122, 34, 45, 46, 48, 57, 
	65, 90, 97, 122, 32, 45, 32, 45, 
	62, 32, 34, 45, 48, 57, 65, 90, 
	97, 122, 34, 45, 46, 48, 57, 65, 
	90, 97, 122, 34, 34, 10, 32, 34, 
	9, 13, 32, 34, 9, 13, 0
};

static const char _dot_single_lengths[] = {
	0, 2, 1, 3, 2, 2, 1, 2, 
	1, 3, 1, 1, 1, 2, 2, 0
};

static const char _dot_range_lengths[] = {
	0, 1, 3, 3, 0, 0, 0, 0, 
	3, 3, 0, 0, 0, 1, 1, 0
};

static const char _dot_index_offsets[] = {
	0, 0, 4, 9, 16, 19, 22, 24, 
	27, 32, 39, 41, 43, 45, 49, 53
};

static const char _dot_indicies[] = {
	1, 2, 1, 0, 3, 3, 3, 3, 
	0, 4, 5, 6, 5, 5, 5, 0, 
	7, 8, 0, 9, 10, 0, 11, 0, 
	11, 12, 0, 13, 13, 13, 13, 0, 
	14, 15, 16, 15, 15, 15, 0, 18, 
	17, 20, 19, 22, 21, 1, 2, 1, 
	0, 23, 24, 23, 0, 25, 0
};

static const char _dot_trans_targs[] = {
	0, 1, 2, 3, 4, 3, 11, 5, 
	6, 5, 6, 7, 8, 9, 14, 9, 
	10, 10, 14, 11, 4, 12, 15, 1, 
	2, 0
};

static const char _dot_trans_actions[] = {
	9, 0, 0, 1, 3, 0, 3, 7, 
	7, 0, 0, 0, 0, 1, 3, 0, 
	3, 0, 0, 0, 0, 0, 5, 11, 
	11, 0
};

static const char _dot_eof_actions[] = {
	0, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 0, 0, 11, 0
};

static const int dot_start = 13;
static const int dot_first_final = 13;
static const int dot_error = 0;

static const int dot_en_consume_line = 12;
static const int dot_en_main = 13;


#line 22 "Dependencies.cpp.rl"

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
  
#line 148 "Dependencies.cpp"
	{
	cs = dot_start;
	}

#line 153 "Dependencies.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dot_trans_keys + _dot_key_offsets[cs];
	_trans = _dot_index_offsets[cs];

	_klen = _dot_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dot_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _dot_indicies[_trans];
	cs = _dot_trans_targs[_trans];

	if ( _dot_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dot_actions + _dot_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 16 "dot-machine.rl"
	{ recipe_name_start(); }
	break;
	case 1:
#line 17 "dot-machine.rl"
	{ recipe_name_stop(); }
	break;
	case 2:
#line 25 "dot-machine.rl"
	{ {cs = 13;goto _again;} }
	break;
	case 3:
#line 31 "dot-machine.rl"
	{ push_recipe(0); }
	break;
	case 4:
#line 36 "dot-machine.rl"
	{ push_recipe(1); }
	break;
	case 5:
#line 38 "dot-machine.rl"
	{ push_dependency(); }
	break;
	case 6:
#line 39 "dot-machine.rl"
	{ p--; {cs = 12;goto _again;} }
	break;
#line 255 "Dependencies.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dot_actions + _dot_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 4:
#line 36 "dot-machine.rl"
	{ push_recipe(1); }
	break;
	case 5:
#line 38 "dot-machine.rl"
	{ push_dependency(); }
	break;
	case 6:
#line 39 "dot-machine.rl"
	{ p--; {cs = 12;	if ( p == pe )
		goto _test_eof;
goto _again;} }
	break;
#line 285 "Dependencies.cpp"
		}
	}
	}

	_out: {}
	}

#line 78 "Dependencies.cpp.rl"

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

