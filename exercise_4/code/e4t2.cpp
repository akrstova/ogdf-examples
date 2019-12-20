#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/GridSifting.h>
#include <ogdf/layered/SplitHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/LongestPathRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
using namespace ogdf;

string base_path = "graph_files/";

// Uncomment the appropriate line based on the parameters combination
string getOutputDir() {
  // return "graph_output/optimalranking_medianheuristic/";
  // return "graph_output/optimalranking_splitheuristic/";
  // return "graph_output/longestpath_medianheuristic/";
  return "graph_output/longestpath_splitheuristic/";
}

int main()
{

    string filenames[20] = {
            "g.10.1.graphml",
            "g.10.22.graphml",
            "g.20.13.graphml",
            "g.20.14.graphml",
            "g.30.15.graphml",
            "g.30.17.graphml",
            "g.40.12.graphml",
            "g.40.8.graphml",
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

    string output_path = getOutputDir();

    for (auto i = 0; i < 20; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        Graph G;
        GraphAttributes GA(G,
                           GraphAttributes::nodeGraphics |
                           GraphAttributes::edgeGraphics |
                           GraphAttributes::nodeLabel |
                           GraphAttributes::edgeStyle |
                           GraphAttributes::nodeStyle |
                           GraphAttributes::nodeTemplate);
        std::cout << string(filenames[i]) << std::endl;
        try {
            if (!GraphIO::read(G, string(filenames[i])))
            {
                std::cerr << "Could not load gml" << std::endl;
            }
        }  catch (const std::exception& e) { // reference to the base of a polymorphic object
            std::cout << e.what() << std::endl;
            return 1;
        }

        SugiyamaLayout SL;
        // SL.setRanking(new OptimalRanking);
        SL.setRanking(new LongestPathRanking);   // uncomment to run
        // SL.setCrossMin(new MedianHeuristic);
        SL.setCrossMin(new SplitHeuristic);

        OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
        ohl->layerDistance(30.0);
        ohl->nodeDistance(25.0);
        ohl->weightBalancing(0.8);
        SL.setLayout(ohl);
        SL.call(GA);
        std::cout << "Number of crossings for file: " << string(filenames[i]) << ": " << to_string(SL.numberOfCrossings()) << std::endl;

        GraphIO::write(GA, string(output_path + filenames[i]) + ".svg", GraphIO::drawSVG);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        std::cout << "Duration for file: " << string(filenames[i]) << ": " << duration << " milliseconds" << std::endl;
    }
    return 0;

}