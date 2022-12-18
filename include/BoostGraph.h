//
// Created by Ian Ferenc Diks on 17.12.22.
//

#ifndef GENETRAILCORE_BOOSTGRAPH_H
#define GENETRAILCORE_BOOSTGRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include <string>


enum vertex_identifier_t { vertex_identifier };
namespace boost {
    BOOST_INSTALL_PROPERTY(vertex, identifier);
}

enum edge_regulation_type_t { edge_regulation_type };
namespace boost {
    BOOST_INSTALL_PROPERTY(edge, regulation_type);
}

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, boost::property<vertex_identifier_t, std::string>, boost::property<edge_regulation_type_t, std::string>> GraphType;
typedef boost::graph_traits<GraphType>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<GraphType>::edge_descriptor edge_descriptor;


#endif//GENETRAILCORE_BOOSTGRAPH_H
