#include <MatrixGraph.hpp>

void MatrixGraph::AddEdge(int from, int to) {
    assertm(0 <= from && from < VerticesCount(), "'from' vertex out of bounds");
    assertm(0 <= to && to < VerticesCount(), "'to' vertex out of bounds");

    edge_matrix[from][to]++;
}

int MatrixGraph::VerticesCount() const {
    return edge_matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (int to = 0; to < edge_matrix[vertex].size(); to++) {
        if (edge_matrix[vertex][to] > 0)
            result.push_back(to);
    }
    
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (int from = 0; from < edge_matrix.size(); from++)
        if (edge_matrix[from][vertex] > 0)
            result.push_back(from);

    return result;
}
