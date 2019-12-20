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
#include <string>

using namespace ogdf;

float computeModularity(ClusterGraph C, Graph originalGraph) {

    // Get all node Degrees
    NodeArray<int> nodeDegrees(originalGraph);
    for (auto *node : originalGraph.nodes)
    {
        nodeDegrees[node] = node->degree();
    }

    // Initialize adjacency matrix with 0s
    NodeArray<NodeArray<int>> adjacencyMat(originalGraph);
    for (node n : originalGraph.nodes)
    {
        adjacencyMat[n].init(originalGraph, 0);
    }

    // Count edges and fill adjacency matrix 
    int edgeCount = 0;
    auto edgeIterator = originalGraph.edges.begin();
    while(edgeIterator != originalGraph.edges.end()) {
        auto current = *edgeIterator;
        adjacencyMat[current->source()][current->target()]++;
        adjacencyMat[current->target()][current->source()]++;
        edgeCount++;
        edgeIterator++;
    }

    float modularity = 0;
    for (auto *v : originalGraph.nodes)
    {
        for (auto *w : originalGraph.nodes)
        {
            bool delta = false; // Kronecker delta, 1 if the nodes belong to the same community, 0 otherwise
            for (auto c = C.firstCluster(); c != C.lastCluster(); c = c->succ())
            {
                // Variables to store whether v and w belong to the same community/cluster
                bool inCommunityV = false;
                bool inCommunityW = false;

                List<node> currentClusterNodes;
                c->getClusterNodes(currentClusterNodes);

                // Compare with nodes of current cluster
                for (auto *node : currentClusterNodes)
                {
                    inCommunityV = inCommunityV || node->index() == v->index();
                    inCommunityW = inCommunityW || node->index() == w->index();
                }
                // Check if both are in the same cluster
                if (inCommunityV & inCommunityW)
                {
                    delta = 1;
                    break;
                }
            }

            if (w->index() == v->index())
                continue;

            // Sum part of the formula
            modularity += ((adjacencyMat[v][w] - (nodeDegrees[v] * nodeDegrees[w] / ((float)(2 * edgeCount)))) * delta);
        }
    }
    return 1 / ((float)(2 * edgeCount)) * modularity;
}

void getConnectedComponents(ClusterGraph C, Graph originalGraph) {
    auto graphIterator = C.clusters.begin();

    while (graphIterator != C.clusters.end()) {
        // Copy the graph before deleting nodes to preserve the original
        GraphCopy graphCopy(originalGraph);

        cluster current = *graphIterator;

        List<node> currentClusterNodes;
        current->getClusterNodes(currentClusterNodes);

        for (auto nodeGraph : originalGraph.nodes)
        {
            bool nodeInCluster = false;
            for (auto nodeCluster : currentClusterNodes)
            {
                if (nodeGraph->index() == nodeCluster->index())
                {
                    nodeInCluster = true;
                    break;
                }
            }

            if (!nodeInCluster)
                graphCopy.delNode(graphCopy.copy(nodeGraph));

        }

        NodeArray<int> connectedCompArr(graphCopy);
        List<node> nodes;
        std::cout << connectedComponents(graphCopy, connectedCompArr, &nodes) << std::endl;
        graphIterator++;
    }

}


int main()
{
    Graph G;
    randomSimpleGraph(G, 30, 70);

    std::cout << "Max. 10 clusters setting " << std::endl;
    for(int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 10);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        std::cout << "Connected components for cluster " << std::to_string(i) << std::endl;
        getConnectedComponents(C, G);
        std::cout << "Modularity " << std::to_string(computeModularity(C, G)) << std::endl;
    }

    std::cout << "--------------------------------------" << std::endl;

    std::cout << "Max. 5 clusters setting " << std::endl;
    for(int i = 0; i < 10; i++) {
        ClusterGraph C(G);
        randomClusterGraph(C, G, 5);
        std::cout << "Graph " << i << " has " << C.numberOfClusters() << " clusters." << std::endl;
        std::cout << "Connected components for cluster " << std::to_string(i) << std::endl;
        getConnectedComponents(C, G);
        std::cout << "Modularity " << std::to_string(computeModularity(C, G)) << std::endl;

    }

    return 0;
}