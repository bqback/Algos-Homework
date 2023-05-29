#include <iostream>
#include <fstream>
#include <climits>
#include <graphs.hpp>

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {

    int vertice_count, edge_count;

    input_stream >> vertice_count;
    assertm(vertice_count >= 0, "Graph can't have negative vertices");
    ListGraph listGraph(vertice_count);

    input_stream >> edge_count;
    assertm(edge_count >= 0, "Graph can't have negative edges");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        input_stream >> from >> to;
        listGraph.AddEdge(from, to);
    }

    MatrixGraph matrixGraph(listGraph);
    SetGraph setGraph(matrixGraph);
    ArcGraph arcGraph(setGraph);

    output_stream << "List graph edges" << std::endl;
    listGraph.PrintEdges();
    output_stream << "Matrix graph edges" << std::endl;
    matrixGraph.PrintEdges();
    output_stream << "Set graph edges" << std::endl;
    setGraph.PrintEdges();
    output_stream << "Arc graph edges" << std::endl;
    arcGraph.PrintEdges();

    return;
}                

int main() {
    std::ifstream input_file;
    // std::ofstream output_file;
    input_file.open("../input.txt");
    // output_file.open("output.txt");
    task(input_file);
    input_file.close();
    // output_file.close();

    return 0;
}