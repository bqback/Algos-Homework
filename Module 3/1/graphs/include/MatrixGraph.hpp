#pragma once
#include <IGraph.hpp>

struct MatrixGraph : public IGraph {
 public:
    MatrixGraph(const int size)
        : edge_matrix(size, std::vector<int>(size, 0))
    {
    }

    MatrixGraph(const IGraph& graph)
        : edge_matrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0)) 
    {
        for (int from = 0; from < graph.VerticesCount(); from++)
            for (int to: graph.GetNextVertices(from)) {
                edge_matrix[from][to]++;
            }
    }

    ~MatrixGraph() {}

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 
 private:
    std::vector<std::vector<int>> edge_matrix;
};