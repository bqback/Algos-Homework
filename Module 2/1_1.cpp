/*
Реализуйте структуру данных типа “множество строк”
    на основе динамической хеш-таблицы с открытой адресацией. 
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления
    значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. 
Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции 
    добавления строки в множество, удаления строки из множества
    и проверки принадлежности данной строки множеству.

Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const size_t DEFAULT_SIZE = 8;
const double MAX_ALPHA = 0.75;

class StringHasher {
 public:
    explicit StringHasher(size_t prime = 71)
        : prime(prime)
        {}

    size_t operator()(const std::string& str) {
        size_t hash = 0;

        for (int i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }

        return hash;
    }
 private:
    size_t prime;
};

template <typename T, typename Hasher>
class HashTable {
 public:
    HashTable(T nil_cell, T del_cell,
                size_t initial_size = DEFAULT_SIZE)
        : nil_cell(nil_cell)
        , del_cell(del_cell)
        , table(initial_size, nil_cell)
        , element_count(0)
        { }
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

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {
    HashTable<std::string, StringHasher> table("\\NIL", "\\DEL");
    char op;
    std::string key;

    while (input_stream >> op >> key) {
        switch (op) {
            case '?': {
                output_stream <<
                    (table.check_element(key) ? "OK" : "FAIL") <<
                std::endl;
                break;
            }
            case '+': {
                output_stream <<
                    (table.add_element(key) ? "OK" : "FAIL") <<
                std::endl;
                break;
            }
            case '-': {
                output_stream <<
                    (table.delete_element(key) ? "OK" : "FAIL") <<
                std::endl;
                break;
            }
        }
    }
}

int main() {
    std::ifstream input_file;
    std::ofstream output_file;
    input_file.open("input.txt");
    output_file.open("output.txt");
    task(input_file, output_file);
    input_file.close();
    output_file.close();

    return 0;
}
