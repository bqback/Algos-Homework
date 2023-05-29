/*
Дан невзвешенный неориентированный граф. 
    В графе может быть несколько кратчайших путей между какими-то вершинами. 
    Найдите количество различных кратчайших путей между заданными вершинами. 
Требования: сложность O(V+E). 
Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.
Формат вывода.
Количество кратчайших путей от u к w.
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <climits>
#include <queue>

#define assertm(exp, msg) assert(((void)msg, exp))

struct IGraph {
    virtual ~IGraph() {}
	
	// Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

void BFS(const IGraph& graph, int from, int to) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    std::vector<int> shortest(graph.VerticesCount(), INT_MAX);
    shortest[from] = 0;
    std::vector<int> path_amount(graph.VerticesCount(), 0);
    path_amount[from] = 1;
    std::vector<int> shortest_from(graph.VerticesCount());
    shortest_from[from] = -1;


    std::queue<int> q;
    q.push(from);
    visited[from] = true;

    while (!q.empty()) {
        int current_vertex = q.front();
        q.pop();

        for (int next_vertex: graph.GetNextVertices(current_vertex)) {
            if (shortest[current_vertex] + 1 < shortest[next_vertex]) {
                    shortest[next_vertex] = shortest[current_vertex] + 1;
                    path_amount[next_vertex] = path_amount[current_vertex];
            } else if (shortest[current_vertex] + 1 == shortest[next_vertex]) {
                    path_amount[next_vertex] += path_amount[current_vertex];
            }
            if (!visited[next_vertex]) {
                visited[next_vertex] = true;
                q.push(next_vertex);
            }
        }
    }
    std::cout << path_amount[to] << std::endl;
}

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {
    int vertice_count, edge_count;
    
    input_stream >> vertice_count;
    assertm(0 < vertice_count && vertice_count <= 50000, "Vertice count must be between 1 and 50000");
    
    input_stream >> edge_count;
    assertm(0 < edge_count && edge_count <= 200000, "Edge count must be between 1 and 50000");
    ListGraph graph(2*edge_count);

    for (int i = 0; i < edge_count; i++) {
        int from, to;
        input_stream >> from >> to;
        assertm(0 <= from && from < vertice_count, "'from' vertice out of bounds");
        assertm(0 <= to && to < vertice_count, "'to' vertice out of bounds");
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    int start, end;
    input_stream >> start >> end;

    BFS(graph, start, end);
}                

int main() {
    // std::ifstream input_file;
    // std::ofstream output_file;
    // input_file.open("input.txt");
    // output_file.open("output.txt");
    // task(input_file, output_file);
    // input_file.close();
    // output_file.close();

    task();

    return 0;
}