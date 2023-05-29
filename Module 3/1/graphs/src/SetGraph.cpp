#include <SetGraph.hpp>

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::add_element(const T& data) {
    if (fill_limit())
        expand();

    size_t hash = hasher(data) % table.size();
    size_t first_deleted = 0;
    int i = 0;

    if (table[hash] == data) {
        return false;
    }

    if (table[hash] == del_cell) {
        first_deleted = hash;
    }

    if (table[hash] != nil_cell) {
        while (i < table.size() - 1) {
            hash = probe(hash, i) % table.size();
            i++;
            if (table[hash] == data)
                return false;
            if (table[hash] == del_cell && first_deleted == 0)
                first_deleted = hash;
            if (table[hash] == nil_cell) {
                first_deleted = 0;
                break;
            }
        }
    }

    if (first_deleted > 0)
        table[first_deleted] = data;
    else
        table[hash] = data;

    element_count++;

    return true;
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::check_element(const T& data) {
    size_t hash = hasher(data) % table.size();

    if (table[hash] == data)
        return true;

    if (table[hash] != nil_cell) {
        int i = 0;
        while (i < table.size() - 1) {
            hash = probe(hash, i) % table.size();
            i++;
            if (table[hash] == data)
                return true;
            if (table[hash] == nil_cell)
                return false;
        }
    }

    return false;
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::delete_element(const T& data) {
    size_t hash = hasher(data) % table.size();

    if (table[hash] == data) {
        table[hash] = del_cell;
        return true;
    }

    if (table[hash] != nil_cell) {
        int i = 0;
        while (i < table.size() - 1) {
            hash = probe(hash, i) % table.size();
            i++;
            if (table[hash] == data) {
                table[hash] = del_cell;
                return true;
            }
            if (table[hash] == nil_cell)
                return false;
        }
    }

    return false;
}

template <typename T, typename Hasher>
void HashTable<T, Hasher>::expand() {
    std::vector<T> new_table(table.size()*2, nil_cell);

    for (int i = 0; i < table.size(); i++) {
        T data = table[i];

        if (data == del_cell)
            continue;

        size_t new_hash = hasher(data) % new_table.size();

        if (new_table[new_hash] != nil_cell) {
            int i = 0;
            while (i < new_table.size() - 1) {
                new_hash = probe(new_hash, i) % new_table.size();
                i++;
                if (new_table[new_hash] == nil_cell)
                    break;
            }
        }

        new_table[new_hash] = data;
    }

    table = std::move(new_table);
}

void SetGraph::AddEdge(int from, int to) {
    assertm(0 <= from && from < VerticesCount(), "'from' vertex out of bounds");
    assertm(0 <= to && to < VerticesCount(), "'to' vertex out of bounds");

    vertices[from].add_element(to);
}

int SetGraph::VerticesCount() const {
    return vertices.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    HashTable<int, IntHasher> vertex_table = vertices[vertex];
    for (int to = 0; to < VerticesCount(); to++)
        if (vertex_table.check_element(to))
            result.push_back(to);
    
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assertm(0 <= vertex && vertex < VerticesCount(), "Vertex out of bounds");

    std::vector<int> result;
    for (int from = 0; from < VerticesCount(); from++) {
        HashTable<int, IntHasher> vertex_table = vertices[from];
        if (vertex_table.check_element(vertex))
            result.push_back(from);
    }

    return result;
}