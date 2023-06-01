/*
Требуется отыскать самый выгодный маршрут между городами. 
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
N ≤ 10000, M ≤ 250000.
Длина каждой дороги ≤ 10000. 
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <climits>
#include <set>

#define assertm(exp, msg) assert(((void)msg, exp))

struct ListGraph {
 public:
    ListGraph(const int size)
        : adjacency_lists(size)
    { 
    }

    ~ListGraph() {}

    void AddEdge(const int from, const int to, const int length);

    int VerticesCount() const;
    
    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const;
    std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const;

 private:
    std::vector<std::vector<std::pair<int, int>>> adjacency_lists;
};

void ListGraph::AddEdge(const int from, const int to, const int length) {
    assertm(0 <= from && from < VerticesCount(), "'from' vertex out of bounds");
    assertm(0 <= to && to < VerticesCount(), "'to' vertex out of bounds");

    adjacency_lists[from].push_back({to, length});
}

int ListGraph::VerticesCount() const {
    return adjacency_lists.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");
    return adjacency_lists[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<std::pair<int, int>> result;
    for (int from = 0; from < adjacency_lists.size(); from++)
        for (std::pair<int, int> to: adjacency_lists[from])
            if (to.first == vertex)
                result.push_back({from, to.second});

    return result;
}

bool relax(int from, int to, int length, std::vector<int>& shortest, std::vector<int>& shortest_from) {
    if (shortest[to] > shortest[from] + length) {
        shortest[to] = shortest[from] + length;
        shortest_from[to] = from;
        return true;
    }
    return false;
}

void find_path(const ListGraph& graph, int from, int to) {
    std::vector<int> shortest(graph.VerticesCount(), INT_MAX);
    shortest[from] = 0;
    std::vector<int> shortest_from(graph.VerticesCount(), -1);


    std::set<std::pair<int, int>> q;
    q.insert({shortest[from], from});

    while (!q.empty()) {
        auto current = q.begin();
        int shortest_to_current = (*current).first;
        int current_vertex = (*current).second; 
        q.erase(current);

        for (std::pair<int, int> edges: graph.GetNextVertices(current_vertex)) {
            int destination = edges.first;
            int path = edges.second;
            int old_shortest = shortest[destination];
            if (old_shortest == INT_MAX) {
                    shortest[destination] = shortest_to_current + path;
                    shortest_from[destination] = current_vertex;
                    q.insert({shortest[destination], destination});
            } else if (relax(current_vertex, destination, path, shortest, shortest_from)) {
                q.erase({old_shortest, destination});
                q.insert({shortest[destination], destination});
            }
        }
    }

    std::cout << shortest[to] << std::endl;
}

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {
    int vertice_count, edge_count;
    
    input_stream >> vertice_count;
    assertm(0 < vertice_count && vertice_count <= 10000, "Vertice count must be between 1 and 10000");
    
    input_stream >> edge_count;
    assertm(0 < edge_count && edge_count <= 250000, "Edge count must be between 1 and 250000");
    ListGraph graph(vertice_count);

    for (int i = 0; i < edge_count; i++) {
        int from, to, length;
        input_stream >> from >> to >> length;
        assertm(0 <= length && length <= 1000000, "Road length must be between 1 and 10000");
        graph.AddEdge(from, to, length);
        graph.AddEdge(to, from, length);
    }

    int start, end;
    input_stream >> start >> end;

    find_path(graph, start, end);
}                

int main() {
    std::ifstream input_file;
    // std::ofstream output_file;
    input_file.open("input.txt");
    // output_file.open("output.txt");
    task(input_file);
    input_file.close();
    // output_file.close();

    // task();

    return 0;
}