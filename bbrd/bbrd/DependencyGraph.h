// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#pragma once

#include "bbrd/Dependencies.h"

#include <ostream>
#include <string_view>
#include <boost/graph/adjacency_list.hpp>


namespace bbrd {


class DependencyGraph
{
public:
  using OutEdgeList = boost::hash_setS;
  using VertexList = boost::vecS;
  using Directed = boost::bidirectionalS;
  using Graph = boost::adjacency_list<OutEdgeList,
                                      VertexList,
                                      Directed>;

  explicit DependencyGraph(Dependencies dependencies);
  DependencyGraph(DependencyGraph&&) = default;
  DependencyGraph(const DependencyGraph&) = delete;
  DependencyGraph& operator=(DependencyGraph&& other) = default;
  DependencyGraph& operator=(const DependencyGraph& other) = delete;

  void list_recipe_depends(
      std::string_view recipe,
      bool reverse,
      std::ostream& out) const;
  void list_adjacent_recipes(
      std::string_view recipe,
      bool reverse,
      std::ostream& out) const;
  void list(std::ostream& out) const;

private:
  Dependencies::Id get_dependency_id_or_throw(std::string_view recipe) const;
  template<typename GraphType>
  void list_adjacent_recipes_of_graph(
      const GraphType& graph,
      std::string_view recipe,
      std::ostream& out) const;

  Dependencies dependencies_;
  Graph graph_;
};


} // namespace bbrd

