// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include <optional>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>


namespace bbrd {


class Dependencies
{
public:
  using Id = std::size_t;
  using DependencyVector = std::vector<std::pair<Id, Id>>;
  using RecipesByStringView = std::unordered_map<std::string_view, Id>;
  using RecipesById = std::vector<std::string_view>;

  explicit Dependencies(std::string buffer)
  : buffer_(std::move(buffer))
  , next_id_(0)
  , dependencies_()
  , recipes_by_string_()
  , recipes_by_id_()
  {
    this->extract_from_dot(this->buffer_);
  }

  Dependencies(Dependencies&& other) = default;
  Dependencies(const Dependencies& other) = delete;
  Dependencies& operator=(Dependencies&& other) = default;
  Dependencies& operator=(const Dependencies& other) = delete;

  DependencyVector::const_iterator begin() const noexcept
  { return this->dependencies_.begin(); }

  DependencyVector::const_iterator end() const noexcept
  { return this->dependencies_.end(); }

  std::string_view get_recipe_name(Id index) const
  { return this->recipes_by_id_.at(index); }

  std::optional<Id> get_recipe_id(std::string_view recipe) const
  {
    auto it = this->recipes_by_string_.find(recipe);
    if( it == this->recipes_by_string_.end() )
      return {};

    return it->second;
  }

  std::size_t distinct_recipe_count() const noexcept
  { return this->next_id_; }

  RecipesByStringView::const_iterator names_begin() const noexcept
  { return this->recipes_by_string_.begin(); }

  RecipesByStringView::const_iterator names_end() const noexcept
  { return this->recipes_by_string_.end(); }

private:
  void extract_from_dot(const std::string& buffer);
  void add_dependency(std::string_view to, std::string_view from);
  Id get_or_create_id(std::string_view recipe);

  std::string buffer_;
  Id next_id_;
  DependencyVector dependencies_;
  RecipesByStringView recipes_by_string_;
  RecipesById recipes_by_id_;
};


} // namespace bbrd

