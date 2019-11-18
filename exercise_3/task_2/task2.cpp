#include <ogdf/basic/graph_generators.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/basic/Array.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/List.h>
#include <string>

using namespace ogdf;

void getDegreeDistribution(ClusterGraph C) {
	Array<int> arr;
    degreeDistribution(C, arr);
    Array<int>::iterator ptr;
    std::cout << "Degree distribution: " << std::endl;
    for (ptr = arr.begin(); ptr != arr.end(); ptr++) {
    	std::cout << *ptr << std::endl;
    }
}

void getConnectedComponents(ClusterGraph C) {
	NodeArray<int> arr;
	List<node> nodes;
	connectedComponents(C, arr);
	NodeArray<int>::iterator ptr;
    std::cout << "Connected components: " << std::endl;
    for (ptr = arr.begin(); ptr != arr.end(); ptr++) {
    	std::cout << *ptr << std::endl;
    }
}

void saveGraph(ClusterGraph C, int graph_number) {
	GraphAttributes GA(C,
					      GraphAttributes::nodeGraphics |
					      GraphAttributes::edgeGraphics |
					      GraphAttributes::nodeLabel |
					      GraphAttributes::edgeStyle |
					      GraphAttributes::nodeStyle |
					      GraphAttributes::nodeTemplate);
    	GraphIO::write(GA, "output-cluster-10max-" + std::to_string(graph_number) + ".svg", GraphIO::drawSVG);
}

int main()
{
    Graph G;
    randomSimpleGraph(G, 30, 70);

    std::cout << "Max. 10 clusters setting " << std::endl;
    for(int i = 0; i < 10; i++) {
    	ClusterGraph C(G);
    	randomClusterGraph(C, G, 9);
    	std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
    	saveGraph(C, i);
    	getDegreeDistribution(C);
		NodeArray<int> narr;
			List<node> nodes;
		connectedComponents(G, narr, &nodes);     // Causes segmentation fault
    }


    return 0;
}