#pragma once
#include <IGraph.hpp>

struct ListGraph : public IGraph {
 public:
    ListGraph(const int size)
        : adjacency_lists(size)
    { 
    }

    ListGraph(const IGraph& graph) 
        : adjacency_lists(graph.VerticesCount())
    {
        for (int from = 0; from < graph.VerticesCount(); from++)
            adjacency_lists[from] = graph.GetNextVertices(from);
    }

    ~ListGraph() {}

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<int>> adjacency_lists;
};