//
// Created by Ian Ferenc Diks on 17.12.22.
//

#ifndef GENETRAILCORE_BOOSTGRAPHPARSER_H
#define GENETRAILCORE_BOOSTGRAPHPARSER_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "BoostGraph.h"
#include "macros.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace genetrailcore {
    class GENETRAILCORE_EXPORT BoostGraphParser {
    public:
        BoostGraphParser() {}
        virtual ~BoostGraphParser() {}

        /**
         * This method checks if there is a vertex with given identifier contained in the graph.
         * If this vertex is not contained a new vertex is added.
         *
         * @param identifier String to specify the searched vertex.
         * @param vertex_map Map to find a vertex based on a given identifier
         * @param graph Boost graph structure
         *
         * @return Pointer to the searched vertex
         */
        template<typename Graph>
        vertex_descriptor checkForVertexInGraph(const std::string &identifier, std::map<std::string, vertex_descriptor> &vertex_map, Graph &graph) {
            typename boost::property_map<Graph, vertex_identifier_t>::type vertex_ids = boost::get(vertex_identifier, graph);
            vertex_descriptor vertex_desc;

            if (vertex_map.find(identifier) != vertex_map.end()) {/// if there is a vertex in the graph with that identifier
                vertex_desc = vertex_map.find(identifier)->second;// return the vertex_desc from the vertex
            } else {                                              /// if there is no vertex with that id inside the graph
                vertex_desc = boost::add_vertex(graph);           // add a new vertex to the graph
                vertex_ids[vertex_desc] = identifier;             // map them for a faster lookup time
                vertex_map[identifier] = vertex_desc;
            }
            return vertex_desc;
        }

        /**
         * This method constructs a graph from a cytoscape .sif file.
         * FORMAT: ID &lt;tab&gt; REGULATION_TYPE &lt;tab&gt; ID &lt;newline&gt;
         *
         * @param filename [.sif] file specifying a network structure
         * @param graph Empty Boost graph structure
         */
        template<typename Graph>
        void readCytoscapeFile(const std::string &fileName, Graph &graph) {
            std::ifstream input_sif;
            std::string current = "";
            std::vector<std::string> entries;

            typename boost::property_map<Graph, edge_regulation_type_t>::type edge_regulations = boost::get(edge_regulation_type, graph);

            vertex_descriptor vertex_desc1, vertex_desc2;
            edge_descriptor edge_desc;

            std::map<std::string, vertex_descriptor> vertex_map;

            input_sif.open(fileName.c_str());
            if (!input_sif) {
                std::cerr << "ERROR: Cannot open: " << fileName << std::endl;
                return;
            }
            while (std::getline(input_sif, current)) {
                if (current != "") {
                    boost::split(entries, current, boost::is_any_of("\t ")) if (entries.size() > 2) {
                        boost::trim(entries[0]);// source node
                        boost::trim(entries[1]);// edge regulation
                        boost::trim(entries[2]);// target node

                        vertex_desc1 = checkForVertexInGraph(entries[0], vertex_map, graph);
                        vertex_desc2 = checkForVertexInGraph(entries[2], vertex_map, graph);

                        bool inserted;

                        boost::tie(edge_desc, inserted) = boost::add_edge(vertex_desc1, vertex_desc2, graph);
                        if (inserted) {
                            edge_regulations[edge_desc] = entries[1];
                        } else {
                            std::cerr << "ERROR: Could not insert edge between " << entries[0] << " and " << entries[2] << fileName << std::endl;
                        }
                    }
                }
            }
        }

        /**
         * This method saves a given graph as a cytoscape .sif file.
         * FORMAT: ID &lt;tab&gt; REGULATION_TYPE &lt;tab&gt; ID &lt;newline&gt;
         *
         * @param filename Name of the resulting .sif file (without ending)
         * @param graph Boost graph structure
         */
        template<typename Graph>
        void writeCytoscapeFile(const std::string &fileName, Graph &graph) {
            std::fstream output_sif;
            output_sif.open(fileName + ".sif", std::ios::out);
            if (!output_sif) {
                std::cerr << "topology::writeCytoscapeFile -> Cannot open output file: " << fileName << " !" << std::endl;
                return;
            }
            typename boost::property_map<Graph, vertex_identifier_t>::type vertex_ids = boost::get(vertex_identifier, graph);
            typename boost::property_map<Graph, edge_regulation_type_t>::type edge_regulations = boost::get(edge_regulation_type, graph);

            typename boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
            typename boost::graph_traits<Graph>::out_edge_iterator vi2, vi2_end;

            // iterate over all vertices now
            for (std::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; vi++) {
                vertex_descriptor vd = *vi;

                // iterate over adjacent vertices of vd
                for (std::tie(vi2, vi2_end) = boost::out_edges(graph); vi2 != vi2_end; vi2++) {
                    output_sif << vertex_ids[vd];// ID of the vertex

                    if (edge_regulations[*vi2] != "") {
                        output_sif << "\t" << edge_regulations[*vi2] << "\t";
                    } else {
                        output_sif << "\tpp\t";
                    }
                    output_sif << vertex_ids[boost::target(*vi2, graph)] << std::endl;
                }
            }
            output_sif.close();
        }
    };
}// namespace genetrailcore

#endif//GENETRAILCORE_BOOSTGRAPHPARSER_H
