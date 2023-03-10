//
// Created by Ian Ferenc Diks on 18.12.22.
//
#include "BoostGraphProcessor.h"

namespace GeneTrail::Core {
    void BoostGraphProcessor::adaptGraph(GraphType &graph, const std::set<std::string> &gene_set) {

        std::set<std::string>::iterator gene_iter;

        typename boost::property_map<GraphType, vertex_identifier_t>::type vertex_ids = boost::get(vertex_identifier, graph);

        // adapt the graph that only genes in the testset are considered
        boost::graph_traits<GraphType>::vertex_iterator vi, vi_end;
        boost::graph_traits<GraphType>::out_edge_iterator vi3, vi3_end;
        boost::graph_traits<GraphType>::in_edge_iterator vi2, vi2_end;
        int deleted_genes = 0;

        //iterate over all vertices to count the number of nodes in the testset
        std::tie(vi, vi_end) = boost::vertices(graph);
        for (auto next = vi; vi != vi_end; vi = next) {
            next++;
            vertex_descriptor vd = *vi;// get the vertex description from the pointer

            gene_iter = gene_set.find(vertex_ids[vd]);// check if the gene of the vertex_description is contained in the gene_set
            bool one_detected = (gene_iter != gene_set.end());

            // unify edges, take all in and all out-edges of the node as a set
            std::set<vertex_descriptor> sources;
            std::set<vertex_descriptor> targets;
            std::set<vertex_descriptor>::iterator sources_iter;
            std::set<vertex_descriptor>::iterator targets_iter;
            std::vector<boost::graph_traits<GraphType>::in_edge_iterator> vit;
            std::vector<boost::graph_traits<GraphType>::out_edge_iterator> vot;

            // if the node has a gene id, add unified edges
            if (!one_detected) {
                deleted_genes++;

                for (std::tie(vi2, vi2_end) = boost::in_edges(vd, graph); vi2 != vi2_end; vi2++) {
                    sources.insert(boost::source(*vi2, graph));
                    vit.push_back(vi2);
                }

                for (std::tie(vi3, vi3_end) = boost::out_edges(vd, graph); vi3 != vi3_end; vi3++) {
                    targets.insert(boost::target(*vi3, graph));
                    vot.push_back(vi3);
                }

                // remove all in edges
                for (auto &elem: vit) {
                    std::cout << "INFO: Edge between " << vertex_ids[boost::source(*elem, graph)] << " and "
                              << vertex_ids[boost::target(*elem, graph)] << " removed" << std::endl;
                    boost::remove_edge(*elem, graph);
                }

                // remove all out edges
                for (auto &elem: vot) {
                    vertex_descriptor vd_target = boost::target(*elem, graph);
                    vertex_descriptor vd_source = boost::source(*elem, graph);

                    if (vd_source != vd_target) {
                        std::cout << "INFO: Edge between " << vertex_ids[vd_source] << " and " << vertex_ids[vd_target] << " removed" << std::endl;
                        boost::remove_edge(*elem, graph);
                    }
                }

                for (sources_iter = sources.begin(); sources_iter != sources.end(); sources_iter++) {
                    for (targets_iter = targets.begin(); targets_iter != targets.end(); targets_iter++) {
                        if (*sources_iter != *targets_iter && *sources_iter != vd && *targets_iter != vd) {
                            boost::add_edge(*sources_iter, *targets_iter, graph);
                            std::cout << "INFO: Edge between " << vertex_ids[*sources_iter] << " and " << vertex_ids[*targets_iter] << " added"
                                      << std::endl;
                        }
                    }
                }
                boost::remove_vertex(*vi, graph);
            }
        }
    }

    std::set<std::string> BoostGraphProcessor::getVertexSet(const GraphType &graph) {
        std::set<std::string> vertex_set;
        boost::graph_traits<GraphType>::vertex_iterator vi, vi_end;
        for (std::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; vi++) { vertex_set.insert(boost::get(vertex_identifier, graph, *vi)); }
        return vertex_set;
    }
}// namespace genetrailcore