#include <chrono>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
using namespace ogdf;
int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    Graph G;
    GraphAttributes GA(G,
      GraphAttributes::nodeGraphics |
      GraphAttributes::edgeGraphics |
      GraphAttributes::nodeLabel |
      GraphAttributes::edgeStyle |
      GraphAttributes::nodeStyle |
      GraphAttributes::nodeTemplate);
    if (!GraphIO::read(GA, G, "Protein_core_2.gml", GraphIO::readGML)) {
        std::cerr << "Could not read Protein_core_2.gml" << std::endl;
        return 1;
    }
    PlanarizationLayout pl;
    PlanarizationGridLayout PGL;
    PGL.call(GA);
    GraphIO::write(GA, "output-planar-protein.svg", GraphIO::drawSVG);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
    std::cout << "The program took " << duration << " ms";
    return 0;
}