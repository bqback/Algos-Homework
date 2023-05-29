#pragma once
#include <IGraph.hpp>

const size_t DEFAULT_SIZE = 8;
const double MAX_ALPHA = 0.75;

struct IntHasher {
    size_t operator()(const int i) {
        return std::hash<int>{}(i);
    }
};

template <typename T, typename Hasher>
class HashTable {
 public:
    HashTable(T nil_cell, T del_cell,
                size_t initial_size)
        : nil_cell(nil_cell)
        , del_cell(del_cell)
        , table(initial_size, nil_cell)
        , element_count(0)
    {
    }
    bool add_element(const T& data);
    bool check_element(const T& data);
    bool delete_element(const T& data);

 private:
    size_t element_count;
    std::vector<T> table;
    Hasher hasher;
    T nil_cell;
    T del_cell;
    bool fill_limit() { return element_count >= MAX_ALPHA*table.size(); }
    size_t probe(size_t hash, int iteration) {
        return (hash + iteration + 1);
    }
    void expand();
};

struct SetGraph : public IGraph {
 public:
    SetGraph(const int size)
        : vertices(size, HashTable<int, IntHasher>(-1, -2, size))
    {
    }

    SetGraph(const IGraph& graph)
        : vertices(graph.VerticesCount(), HashTable<int, IntHasher>(-1, -2, graph.VerticesCount()))
    {
        for (int from = 0; from < graph.VerticesCount(); from++)
            for (int to: graph.GetNextVertices(from)) {
                vertices[from].add_element(to);
            }
    }

    ~SetGraph() {}

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 
 private:
    int vertex_count;
    std::vector<HashTable<int, IntHasher>> vertices;
};