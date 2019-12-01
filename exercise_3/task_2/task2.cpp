#include <ogdf/basic/graph_generators.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/basic/Array.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/List.h>
#include <ogdf/graphalg/ShortestPathAlgorithms.h>
#include <ogdf/basic/SList.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Array.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/basic/CombinatorialEmbedding.h>
#include <ogdf/graphalg/Dijkstra.h>
#include <ogdf/basic/GraphCopy.h>
#include <ogdf/basic/DualGraph.h>
#include <ogdf/graphalg/MinSTCutModule.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/basic/Queue.h>
#include <ogdf/cluster/ClusterGraph.h>
#include <string>

using namespace ogdf;

// Not used
struct TCost {
    int cost;

    TCost(int cost) {
        this->cost = cost;
    }
};

void getClusterDegreeDistribution(cluster c) {
    List<node> clusterNodes;
    c->getClusterNodes(clusterNodes);
    for (node n : clusterNodes) {
        std::cout << n->degree() << " ";
    }
    std::cout << "\n";
}

void getDegreeDistribution(ClusterGraph C) {
    int count = 0;
    Array<int> arr;
    std::cout << "Degree distribution of cluster " << std::to_string(count) << std::endl;
    cluster c = C.firstCluster();
    getClusterDegreeDistribution(c);
    count++;
    while((c = c->succ())) {
        std::cout << "Degree distribution of cluster " << std::to_string(count) << std::endl;
        getClusterDegreeDistribution(c);
        count++;
    }
}

void getConnectedComponents(ClusterGraph C) {
    auto narr = new NodeArray<int>(C);
    List<node> nodes;
    std::cout << "The connected components are:" << std::endl;
    std::cout << connectedComponents(C, *narr, &nodes) << std::endl;
}

void saveGraph(ClusterGraph C, int setting, int graph_number) {
    // Filename should be in the form of: output-cluster-10max-cluster0.gml
    GraphIO::writeGML(C, "output-cluster-" + std::to_string(setting) + "max-cluster" + std::to_string(graph_number) + ".gml");
}

void getGraphDiameter(Graph G) {
    int sum = 0;
    NodeArray<NodeArray<int>> matrix(G);
    for (node n : G.nodes)
    {
        matrix[n].init(G, -100000);
        matrix[n][n] = 0;
    }
    std::cout << "initialized" << std::endl;
    bfs_SPAP(G, matrix, 1);
    int max = -100000;
    for (auto i = matrix.begin(); i != matrix.end(); i++)
    {
        NodeArray<int> row = *i;
        for (auto j = row.begin(); j != row.end(); j++)
        {
            int pathLength = *j;
            sum += pathLength;
            if (pathLength > max)
            {
                max = pathLength;
            }
        }
    }
    std::cout << "The diameter of the graph is: " << max << std::endl;
    std::cout << "The average path length is: " << sum / (G.numberOfNodes()*G.numberOfNodes()) << std::endl;
}

int main()
{
    Graph G;
    randomSimpleGraph(G, 30, 70);
    getGraphDiameter(G);

    std::cout << "Max. 10 clusters setting " << std::endl;
    for(int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 9);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        saveGraph(C, i, 10);
        getDegreeDistribution(C);
        getConnectedComponents(C);
    }

    std::cout << "--------------------------------------" << std::endl;

    std::cout << "Max. 5 clusters setting " << std::endl;
    for(int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 4);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        saveGraph(C, i, 5);
        getDegreeDistribution(C);
        getConnectedComponents(C);
    }


    return 0;
}