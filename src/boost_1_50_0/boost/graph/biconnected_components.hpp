// Copyright (c) Jeremy Siek 2001
// Copyright (c) Douglas Gregor 2004
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// NOTE: this final is generated by libs/graph/doc/biconnected_components.w

#ifndef BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP
#define BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP

#include <stack>
#include <vector>
#include <algorithm> // for std::min and std::max
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/concept/assert.hpp>

namespace boost
{
  namespace detail
  {
    template<typename ComponentMap, typename DiscoverTimeMap,
             typename LowPointMap, typename PredecessorMap,
             typename OutputIterator, typename Stack,
             typename ArticulationVector, typename IndexMap,
             typename DFSVisitor>
    struct biconnected_components_visitor : public dfs_visitor<>
    {
      biconnected_components_visitor
        (ComponentMap comp, std::size_t& c, 
         std::size_t& children_of_root, DiscoverTimeMap dtm,
         std::size_t& dfs_time, LowPointMap lowpt, PredecessorMap pred,
         OutputIterator out, Stack& S,
         ArticulationVector& is_articulation_point, IndexMap index_map,
         DFSVisitor vis)
          : comp(comp), c(c), children_of_root(children_of_root),
            dtm(dtm), dfs_time(dfs_time), lowpt(lowpt),
            pred(pred), out(out), S(S),
            is_articulation_point(is_articulation_point),
            index_map(index_map), vis(vis) { }

      template <typename Vertex, typename Graph>
      void initialize_vertex(const Vertex& u, Graph& g)
      {
        put(pred, u, u);
        vis.initialize_vertex(u, g);
      }

      template <typename Vertex, typename Graph>
      void start_vertex(const Vertex& u, Graph& g)
      {
        children_of_root = 0;
        vis.start_vertex(u, g);
      }

      template <typename Vertex, typename Graph>
      void discover_vertex(const Vertex& u, Graph& g)
      {
        put(dtm, u, ++dfs_time);
        put(lowpt, u, get(dtm, u));
        vis.discover_vertex(u, g);
      }

      template <typename Edge, typename Graph>
      void examine_edge(const Edge& e, Graph& g)
      {
        vis.examine_edge(e, g);
      }

      template <typename Edge, typename Graph>
      void tree_edge(const Edge& e, Graph& g)
      {
        typename boost::graph_traits<Graph>::vertex_descriptor src = source(e, g);
        typename boost::graph_traits<Graph>::vertex_descriptor tgt = target(e, g);

        S.push(e);
        put(pred, tgt, src);
        if ( get(pred, src) == src ) {
          ++children_of_root;
        }
        vis.tree_edge(e, g);
      }

      template <typename Edge, typename Graph>
      void back_edge(const Edge& e, Graph& g)
      {
        BOOST_USING_STD_MIN();

        typename boost::graph_traits<Graph>::vertex_descriptor src = source(e, g);
        typename boost::graph_traits<Graph>::vertex_descriptor tgt = target(e, g);
        if ( tgt != get(pred, src) ) {
          S.push(e);
          put(lowpt, src,
              min BOOST_PREVENT_MACRO_SUBSTITUTION(get(lowpt, src),
                                                   get(dtm, tgt)));
        }
        vis.back_edge(e, g);
      }

      template <typename Edge, typename Graph>
      void forward_or_cross_edge(const Edge& e, Graph& g)
      {
        vis.forward_or_cross_edge(e, g);
      }

      template <typename Vertex, typename Graph>
      void finish_vertex(const Vertex& u, Graph& g)
      {
        BOOST_USING_STD_MIN();
        Vertex parent = get(pred, u);
        if (parent == u) { // Root of tree is special
          is_articulation_point[get(index_map, u)] = (children_of_root > 1);
        } else {
          put(lowpt, parent,
              min BOOST_PREVENT_MACRO_SUBSTITUTION(get(lowpt, parent),
                                                 get(lowpt, u)));
          if ( get(lowpt, u) >= get(dtm, parent) ) {
            is_articulation_point[get(index_map, parent)] = true;
            while ( get(dtm, source(S.top(), g)) >= get(dtm, u) ) {
              put(comp, S.top(), c);
              S.pop();
            }
            assert (source(S.top(), g) == parent);
            assert (target(S.top(), g) == u);
            put(comp, S.top(), c);
            S.pop();
            ++c;
          }
        }
        if ( is_articulation_point[get(index_map, u)] ) {
          *out++ = u;
        }
        vis.finish_vertex(u, g);
      }

      ComponentMap comp;
      std::size_t& c;
      std::size_t& children_of_root;
      DiscoverTimeMap dtm;
      std::size_t& dfs_time;
      LowPointMap lowpt;
      PredecessorMap pred;
      OutputIterator out;
      Stack& S;
      ArticulationVector& is_articulation_point;
      IndexMap index_map;
      DFSVisitor vis;
    };

  template<typename Graph, typename ComponentMap, typename OutputIterator,
        typename VertexIndexMap, typename DiscoverTimeMap, typename LowPointMap,
        typename PredecessorMap, typename DFSVisitor>
  std::pair<std::size_t, OutputIterator>
    biconnected_components_impl(const Graph & g, ComponentMap comp,
        OutputIterator out, VertexIndexMap index_map, DiscoverTimeMap dtm,
        LowPointMap lowpt, PredecessorMap pred, DFSVisitor dfs_vis)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename graph_traits<Graph>::edge_descriptor edge_t;
    BOOST_CONCEPT_ASSERT(( VertexListGraphConcept<Graph> ));
    BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept<Graph> ));
    BOOST_CONCEPT_ASSERT(( WritablePropertyMapConcept<ComponentMap, edge_t> ));
    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<DiscoverTimeMap,
                                                  vertex_t> ));
    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<LowPointMap, vertex_t > ));
    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<PredecessorMap,
                                                  vertex_t> ));

    std::size_t num_components = 0;
    std::size_t children_of_root;
    std::size_t dfs_time = 0;
    std::stack<edge_t> S;
	std::vector<char> is_articulation_point(num_vertices(g));

    biconnected_components_visitor<ComponentMap, DiscoverTimeMap,
        LowPointMap, PredecessorMap, OutputIterator, std::stack<edge_t>, 
        std::vector<char>, VertexIndexMap, DFSVisitor>
    vis(comp, num_components, children_of_root, dtm, dfs_time,
        lowpt, pred, out, S, is_articulation_point, index_map, dfs_vis);

    depth_first_search(g, visitor(vis).vertex_index_map(index_map));

    return std::pair<std::size_t, OutputIterator>(num_components, vis.out);
  }

    template <typename PredecessorMap>
    struct bicomp_dispatch3
    {
  template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename LowPointMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph & g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, LowPointMap lowpt, 
          const bgl_named_params<P, T, R>& params, PredecessorMap pred)
      {
        return biconnected_components_impl
                (g, comp, out, index_map, dtm, lowpt, pred,
                 choose_param(get_param(params, graph_visitor),
                    make_dfs_visitor(null_visitor())));
      }
    };
    
    template <>
    struct bicomp_dispatch3<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename LowPointMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph & g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, LowPointMap lowpt, 
          const bgl_named_params<P, T, R>& params, 
          error_property_not_found)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> pred(num_vertices(g));
    vertex_t vert = graph_traits<Graph>::null_vertex();

        return biconnected_components_impl
                (g, comp, out, index_map, dtm, lowpt, 
              make_iterator_property_map(pred.begin(), index_map, vert),
                 choose_param(get_param(params, graph_visitor),
                    make_dfs_visitor(null_visitor())));
  }
    };

    template <typename LowPointMap>
    struct bicomp_dispatch2
    {
  template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename P, typename T, typename R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, const bgl_named_params<P, T, R>& params, 
          LowPointMap lowpt)
      {
        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_predecessor_t>::type dispatch_type;

        return bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, index_map, dtm, lowpt, params, 
             get_param(params, vertex_predecessor));
      }
    };


    template <>
    struct bicomp_dispatch2<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename P, typename T, typename R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, const bgl_named_params<P, T, R>& params, 
          error_property_not_found)
  {
    typedef typename graph_traits<Graph>::vertices_size_type
      vertices_size_type;
    std::vector<vertices_size_type> lowpt(num_vertices(g));
        vertices_size_type vst(0);

        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_predecessor_t>::type dispatch_type;
  
        return bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, index_map, dtm,
             make_iterator_property_map(lowpt.begin(), index_map, vst),
             params, get_param(params, vertex_predecessor));
      }
    };

    template <typename DiscoverTimeMap>
    struct bicomp_dispatch1
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply(const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          const bgl_named_params<P, T, R>& params, DiscoverTimeMap dtm)
      {
        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_lowpoint_t>::type dispatch_type;

        return bicomp_dispatch2<dispatch_type>::apply
            (g, comp, out, index_map, dtm, params, 
             get_param(params, vertex_lowpoint));
      }
    };

    template <>
    struct bicomp_dispatch1<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply(const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          const bgl_named_params<P, T, R>& params, error_property_not_found)
      {
        typedef typename graph_traits<Graph>::vertices_size_type
            vertices_size_type;
        std::vector<vertices_size_type> discover_time(num_vertices(g));
    vertices_size_type vst(0);

        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_lowpoint_t>::type dispatch_type;

        return bicomp_dispatch2<dispatch_type>::apply
            (g, comp, out, index_map, 
              make_iterator_property_map(discover_time.begin(), index_map, vst),
             params, get_param(params, vertex_lowpoint));
      }
    };

  }

  template<typename Graph, typename ComponentMap, typename OutputIterator,
      typename DiscoverTimeMap, typename LowPointMap>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, 
      OutputIterator out, DiscoverTimeMap dtm, LowPointMap lowpt)
  {
    typedef detail::error_property_not_found dispatch_type;

    return detail::bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, 
             get(vertex_index, g), 
             dtm, lowpt, 
             bgl_named_params<int, buffer_param_t>(0), 
             detail::error_property_not_found());
  }

  template <typename Graph, typename ComponentMap, typename OutputIterator,
      typename P, typename T, typename R>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out, 
      const bgl_named_params<P, T, R>& params)
  {
    typedef typename property_value< bgl_named_params<P,T,R>,
        vertex_discover_time_t>::type dispatch_type;

    return detail::bicomp_dispatch1<dispatch_type>::apply(g, comp, out, 
        choose_const_pmap(get_param(params, vertex_index), g, vertex_index), 
        params, get_param(params, vertex_discover_time));
  }

  template < typename Graph, typename ComponentMap, typename OutputIterator>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out)
  {
    return biconnected_components(g, comp, out,  
        bgl_named_params<int, buffer_param_t>(0));
  }

  namespace graph_detail {
    struct dummy_output_iterator
    {
      typedef std::output_iterator_tag iterator_category;
      typedef void value_type;
      typedef void pointer;
      typedef void difference_type;

      struct reference {
        template<typename T>
        reference& operator=(const T&) { return *this; }
      };

      reference operator*() const { return reference(); }
      dummy_output_iterator& operator++() { return *this; }
      dummy_output_iterator operator++(int) { return *this; }
    };
  } // end namespace graph_detail

  template <typename Graph, typename ComponentMap,
      typename P, typename T, typename R>
  std::size_t
  biconnected_components(const Graph& g, ComponentMap comp, 
      const bgl_named_params<P, T, R>& params)
  {
    return biconnected_components(g, comp,
        graph_detail::dummy_output_iterator(), params).first;
  }

  template <typename Graph, typename ComponentMap>
  std::size_t
  biconnected_components(const Graph& g, ComponentMap comp)
  {
    return biconnected_components(g, comp,
                                  graph_detail::dummy_output_iterator()).first;
  }

  template<typename Graph, typename OutputIterator, 
      typename P, typename T, typename R>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out, 
      const bgl_named_params<P, T, R>& params)
  {
    return biconnected_components(g, dummy_property_map(), out, 
        params).second;
  }

  template<typename Graph, typename OutputIterator>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out)
  {
    return biconnected_components(g, dummy_property_map(), out, 
        bgl_named_params<int, buffer_param_t>(0)).second;
  }

}                               // namespace boost

#endif  /* BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP */
