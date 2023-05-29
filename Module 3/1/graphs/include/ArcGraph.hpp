#pragma once
#include <IGraph.hpp>

struct ArcGraph : public IGraph {
 public:
    ArcGraph(const int size)
        : vertex_count(size)
    {
    }

    ArcGraph(const IGraph& graph)
        : vertex_count(graph.VerticesCount())
    {
        for (int from = 0; from < graph.VerticesCount(); from++)
            for (int to: graph.GetNextVertices(from)) {
                edges.push_back({from, to});
            }
    }

    ~ArcGraph() {}

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 
 private:
    int vertex_count;
    std::vector<std::pair<int, int>> edges;
};