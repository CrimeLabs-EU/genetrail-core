//
// Created by Ian Ferenc Diks on 18.12.22.
//

#ifndef GENETRAILCORE_BOOSTGRAPHPROCESSOR_H
#define GENETRAILCORE_BOOSTGRAPHPROCESSOR_H

#include "BoostGraph.h"
#include "macros.h"

#include <iostream>
#include <set>
#include <vector>

namespace genetrailcore {

    class GENETRAILCORE_EXPORT BoostGraphProcessor {
    public:
        /**
         * This method adepts a given graph.
         * If vertices of the graph are not contained in the gene set they are deleted and their predecessors and successors are connected.
         *
         * @param graph Boost graph structure
         * @param gene_set set of gene that are allowed in the graph
         */
        void adaptGraph(GraphType &graph, const std::set<std::string> &gene_set);

        /**
         * This method returns a set of vertex identifiers that are contained in the graph.
         *
         * @param graph Boost graph
         * @return Set of vertex identifiers that are contained in the graph
         */
        std::set<std::string> getVertexSet(const GraphType &graph);
    };

}// namespace genetrailcore

#endif//GENETRAILCORE_BOOSTGRAPHPROCESSOR_H
