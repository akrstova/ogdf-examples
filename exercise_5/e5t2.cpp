#include <chrono>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/basic/LayoutStatistics.h>
#include <ogdf/energybased/StressMinimization.h>
using namespace ogdf;

int main()
{
    string files[] = {
      "g.10.1.graphml",
      "g.10.22.graphml",
      "g.20.13.graphml",
      "g.20.14.graphml",
      "g.30.15.graphml",
      "g.30.17.graphml",
      "g.40.8.graphml",
      "g.40.12.graphml",
      "g.50.7.graphml",
      "g.50.8.graphml",
      "g.60.0.graphml",
      "g.60.1.graphml",
      "g.70.1.graphml",
      "g.71.2.graphml",
      "g.80.6.graphml",
      "g.81.9.graphml",
      "g.90.1.graphml",
      "g.90.2.graphml",
      "g.100.1.graphml",
      "g.100.3.graphml"
    };
    for (int i = 0; i < 20; i++) {
      Graph G;
      GraphAttributes GA(G,
        GraphAttributes::nodeGraphics |
        GraphAttributes::edgeGraphics |
        GraphAttributes::nodeLabel |
        GraphAttributes::nodeType |
        GraphAttributes::edgeType |
        GraphAttributes::edgeStyle |
        GraphAttributes::nodeStyle |
        GraphAttributes::nodeTemplate);
      if (!GraphIO::read(G, files[i])) {
          std::cerr << "Could not read " << files[i] << std::endl;
          return 1;
      }
      // Copy the graph for the SM layout
      GraphCopy G1(G);
      GraphAttributes GA1(G, GraphAttributes::nodeGraphics |
        GraphAttributes::edgeGraphics |
        GraphAttributes::nodeLabel |
        GraphAttributes::nodeType |
        GraphAttributes::edgeType |
        GraphAttributes::edgeStyle |
        GraphAttributes::nodeStyle |
        GraphAttributes::nodeTemplate);

      PlanarizationLayout pl;
      OrthoLayout *ol = new OrthoLayout;
      pl.setPlanarLayouter(ol);
      pl.call(GA);
      std::cout << "Number of nodes " << G.numberOfNodes() << std::endl;
      std::cout << "Number of edges " << G.numberOfEdges() << std::endl;
      std::cout << "Number of bends for graph " << LayoutStatistics::numberOfBends(GA) << std::endl;
      std::cout << "Bends per edge " << std::endl;
      for (edge e : G.edges) {
        std::cout << GA.bends(e).size() << ", ";
      }
      std::cout << std::endl;

      GraphIO::write(GA, "output_ortho_" + files[i] + ".svg", GraphIO::drawSVG);

      StressMinimization sm;
      sm.call(GA1);
      GraphIO::write(GA1, "output_sm_" + files[i] + ".svg", GraphIO::drawSVG);

    }

    
    return 0;
}