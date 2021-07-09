// Author: Thomas Trapp - https://thomastrapp.com/
// License: MIT

#include "bbrd/DependencyGraph.h"
#include "bbrd/Dependencies.h"

#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/range/iterator_range.hpp>


namespace {


template<typename OutputIterator>
class DependencyRecorder : public boost::bfs_visitor<>
{
public:
  explicit DependencyRecorder(OutputIterator& out)
  : out_(out)
  {}

  template<class Graph>
  void discover_vertex(bbrd::Dependencies::Id id, const Graph&)
  {
    *(this->out_++) = id;
  }

private:
  OutputIterator& out_;
};


} // namespace


namespace bbrd {


DependencyGraph::DependencyGraph(Dependencies dependencies)
: dependencies_(std::move(dependencies))
, graph_(
    this->dependencies_.begin(),
    this->dependencies_.end(),
    this->dependencies_.distinct_recipe_count())
{
}

void DependencyGraph::list_recipe_depends(
    std::string_view recipe,
    bool reverse,
    std::ostream& out) const
{
  auto id = this->get_dependency_id_or_throw(recipe);
  std::vector<Dependencies::Id> dependencies;
  auto it = std::back_inserter(dependencies);
  DependencyRecorder dependency_recorder(it);

  if( reverse )
  {
    boost::breadth_first_search(
        boost::make_reverse_graph(this->graph_),
        id,
        boost::visitor(dependency_recorder));
  }
  else
  {
    boost::breadth_first_search(
        this->graph_,
        id,
        boost::visitor(dependency_recorder));
  }

  if( dependencies.size() )
  {
    assert(dependencies.front() == id);
    for(auto recipe_id = dependencies.rbegin() ;
        recipe_id != (dependencies.rend() - 1);
        ++recipe_id)
      out << this->dependencies_.get_recipe_name(*recipe_id) << "\n";
  }
}

void DependencyGraph::list(std::ostream& out) const
{
  auto it = this->dependencies_.names_begin();
  for(; it != this->dependencies_.names_end(); ++it)
    out << it->first << "\n";
}

void DependencyGraph::list_adjacent_recipes(
    std::string_view recipe,
    bool reverse,
    std::ostream& out) const
{
  if( reverse )
    this->list_adjacent_recipes_of_graph(
        boost::make_reverse_graph(this->graph_),
        recipe,
        out);
  else
    this->list_adjacent_recipes_of_graph(
        this->graph_,
        recipe,
        out);
}

Dependencies::Id DependencyGraph::get_dependency_id_or_throw(
    std::string_view recipe) const
{
  auto id = this->dependencies_.get_recipe_id(recipe);
  if( !id )
    throw std::runtime_error(std::string("recipe not found: ").append(recipe));

  return *id;
}

template<typename GraphType>
void DependencyGraph::list_adjacent_recipes_of_graph(
    const GraphType& graph,
    std::string_view recipe,
    std::ostream& out) const
{
  auto id = this->get_dependency_id_or_throw(recipe);
  auto it_pair = boost::adjacent_vertices(id, graph);
  for(auto recipe_id : boost::make_iterator_range(it_pair))
    out << this->dependencies_.get_recipe_name(recipe_id) << "\n";
}


} // namespace bbrd

