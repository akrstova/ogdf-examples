#include <ogdf/energybased/SpringEmbedderKK.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/Array.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/List.h>
#include <ogdf/basic/SList.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Array.h>
#include <ogdf/basic/GraphCopy.h>
#include <ogdf/basic/Queue.h>
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/basic/LayoutStatistics.h>
#include <string>
#include <random>
#include <time.h>

using namespace ogdf;

int main() {
	int seeds[] = {100, 200, 300, 150, 350};

	string graph_files[] =  { 
		                "grafo121.20.graphml",
                        "grafo117.20.graphml",
                        "grafo1182.50.graphml",
                        "grafo1307.50.graphml",
                        "grafo8674.100.graphml",
                        "grafo8564.100.graphml",
                        };


	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			auto start = std::chrono::high_resolution_clock::now();
		    Graph G;
		    GraphAttributes GA(G,
		      GraphAttributes::nodeGraphics |
		      GraphAttributes::edgeGraphics |
		      GraphAttributes::nodeLabel |
		      GraphAttributes::edgeStyle |
		      GraphAttributes::nodeStyle |
		      GraphAttributes::nodeTemplate);
		    if (!GraphIO::read(G, graph_files[i])) {
		        std::cerr << "Could not load " << graph_files[i] << std::endl;
		        return 1;
		    }
		    std::cout << "output_" + graph_files[i] + "_" + std::to_string(j) + ".svg" << std::endl; 

		    FMMMLayout fmmm;
		    fmmm.randSeed(seeds[j]);
		    fmmm.call(GA);
		    GraphIO::write(GA, "output_" + graph_files[i] + "_" + std::to_string(j) + ".svg", GraphIO::drawSVG);
		    auto end = std::chrono::high_resolution_clock::now();
		    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
		    std::cout << "The algorithm took " << duration << " milliseconds" << std::endl;
		    std::cout << "Number of crossings: " << ogdf::LayoutStatistics::numberOfCrossings(GA) << std::endl;
		    std::cout << "Total edge length: " << ogdf::LayoutStatistics::edgeLengths(GA) << std::endl;

		}
	}
    return 0;
}