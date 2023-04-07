/*
4.1 Слияние массивов.
Напишите программу, которая использует кучу для 
    слияния K отсортированных массивов суммарной длиной N.
Требования: время работы O(N * logK). Ограничение на размер кучи O(K)..
Формат входных данных: Сначала вводится количество массивов K. 
    Затем по очереди размер каждого массива и элементы массива. 
    Каждый массив упорядочен по возрастанию.
Формат выходных данных: Итоговый отсортированный массив.
*/
#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

struct ArrayIterator {
    int* array;
    int size;
    int current_pos;
};

struct ArrayIteratorComparator {
    bool operator()(const ArrayIterator& l, const ArrayIterator& r) {
        return l.array[l.current_pos] < r.array[r.current_pos];
    }
};

template <typename T, typename Comparator>
class Heap {
 public:
    Heap()
        : heap_size(1)
        , element_count(0) {
        elements = new T[heap_size];
    }
    ~Heap() {
        delete elements;
    }
    int extract_top();
    void add_element(T element);
    bool has_elements() { return element_count != 0; }
 private:
    void sift_up(int index);
    void sift_down(int index);
    void delete_last_element();
    void grow();
    int heap_size;
    int element_count;
    Comparator cmp;
    T *elements;
};

template <typename T, typename Comparator>
void Heap<T, Comparator>::delete_last_element() {
    T* temp = new T[element_count];
    std::copy(elements, elements + element_count - 1, temp);
    delete elements;
    elements = temp;
}

template <typename T, typename Comparator>
int Heap<T, Comparator>::extract_top() {
    T root = elements[0];
    int value = root.array[root.current_pos];
    root.current_pos++;
    elements[0] = elements[element_count-1];
    sift_down(0);
    delete_last_element();
    element_count--;
    if (root.current_pos != root.size)
        add_element(root);

    return value;
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::sift_up(int index) {
    int parent_index = (index - 1)/2;

    while (index != 0 && cmp(elements[index], elements[parent_index])) {
        std::swap(elements[index], elements[parent_index]);
        index = parent_index;
        parent_index = (index - 1)/2;
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::sift_down(int index) {
    int left_index = 2*index + 1;
    int right_index = 2*index + 2;
    int target_index = index;

    // Сразу выбираем наименьшего из чайлд элементов
    if (left_index < element_count &&
        cmp(elements[left_index], elements[index]))
        target_index = left_index;
    // Сравнение происходит либо с исходным элементом,
    // либо сразу с левым чайлдом
    if (right_index < element_count &&
        cmp(elements[right_index], elements[target_index]))
        target_index = right_index;
    if (target_index != index) {
        std::swap(elements[index], elements[target_index]);
        sift_down(target_index);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::add_element(T new_element) {
    if (element_count == heap_size)
        grow();

    int insertion_index = element_count;
    elements[insertion_index] = new_element;
    element_count++;

    sift_up(insertion_index);
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::grow() {
    int new_size = heap_size*2;
    T* new_heap = new T[new_size];
    std::copy(elements, elements+heap_size, new_heap);
    delete elements;
    elements = new_heap;
    heap_size = new_size;
}

template <typename T, typename Comparator>
void fuse(Heap<T, Comparator>& heap) {
    while (heap.has_elements())
        std::cout << heap.extract_top() << " ";
    std::cout << std::endl;
}

int main() {
    int k;

    std::cin >> k;

    Heap<ArrayIterator, ArrayIteratorComparator> heap;

    assertm(k > 0, "Number of elements in heap must be greater than 0");
    for (int i = 0; i < k; i++) {
        int array_size;
        std::cin >> array_size;
        assertm(array_size > 0, "Array size must be greater than 0");
        int* array = new int[array_size];
        ArrayIterator element{array, array_size, 0};
        for (int j = 0; j < array_size; j++) {
            std::cin >> element.array[j];
            if (j > 1)
                assertm(element.array[j-1] <= element.array[j],
                        "Array must be sorted in non-descending order");
        }
        heap.add_element(element);
    }

    fuse(heap);

    return 0;
}
