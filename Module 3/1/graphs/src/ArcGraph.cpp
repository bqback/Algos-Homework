#include <ArcGraph.hpp>

void ArcGraph::AddEdge(int from, int to) {
    assertm(0 <= from && from < VerticesCount(), "'from' vertex out of bounds");
    assertm(0 <= to && to < VerticesCount(), "'to' vertex out of bounds");

    edges.push_back({from, to});
}

int ArcGraph::VerticesCount() const {
    return vertex_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (std::pair<int, int> edge: edges)
        if (edge.first == vertex)
            result.push_back(edge.second);
    
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (std::pair<int, int> edge: edges)
        if (edge.second == vertex)
            result.push_back(edge.first);

    return result;
}
