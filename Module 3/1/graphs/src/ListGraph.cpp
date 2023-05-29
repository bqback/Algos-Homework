#include <ListGraph.hpp>

void ListGraph::AddEdge(int from, int to) {
    assertm(0 <= from && from < VerticesCount(), "'from' vertex out of bounds");
    assertm(0 <= to && to < VerticesCount(), "'to' vertex out of bounds");

    adjacency_lists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjacency_lists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");
    return adjacency_lists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (int from = 0; from < adjacency_lists.size(); from++)
        for (int to: adjacency_lists[from])
            if (to == vertex)
                result.push_back(from);

    return result;
}
