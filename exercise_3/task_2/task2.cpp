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
    std::cout << "The array elements are: " << std::endl;
    for (ptr = arr.begin(); ptr != arr.end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
}

void getConnectedComponents(ClusterGraph C) {
    auto narr = new NodeArray<int>(C);
    List <node> nodes;
    std::cout << "The connected components are:" << std::endl;
    std::cout << connectedComponents(C, *narr, &nodes) << std::endl;
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

int main() {
    Graph G;
    randomSimpleGraph(G, 30, 70);

    std::cout << "Max. 10 clusters setting " << std::endl;
    for (int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 9);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        saveGraph(C, i);
        getDegreeDistribution(C);
        getConnectedComponents(C);
    }

    std::cout << "--------------------------------------" << std::endl;

    std::cout << "Max. 5 clusters setting " << std::endl;
    for (int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 4);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        saveGraph(C, i);
        getDegreeDistribution(C);
        getConnectedComponents(C);
    }
    return 0;
}