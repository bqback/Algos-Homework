/*
5_4. Закраска прямой 2.
На числовой прямой окрасили N отрезков. 
Известны координаты левого и правого концов каждого отрезка (Li и Ri). 
Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
*/
#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

struct Line {
    int coordinate;
    int change;
};

struct LineComparator {
    bool operator()(const Line& lhs, const Line& rhs) {
        return lhs.coordinate <= rhs.coordinate;
    }
};

template <typename T, typename Comparator>
void merge(T* left_array, const int left_length,
            T* right_array, const int right_length,
            T* buffer, Comparator cmp) {
    int left_index = 0, right_index = 0, merged_index = 0;

    while (left_index < left_length && right_index < right_length) {
        if (cmp(left_array[left_index], right_array[right_index])) {
            buffer[merged_index] = left_array[left_index];
            left_index++;
        } else {
            buffer[merged_index] = right_array[right_index];
            right_index++;
        }
        merged_index++;
    }

    while (left_index < left_length) {
        buffer[merged_index] = left_array[left_index];
        left_index++;
        merged_index++;
    }

    while (right_index < right_length) {
        buffer[merged_index] = right_array[right_index];
        right_index++;
        merged_index++;
    }
}

template <typename T, typename Comparator>
void merge_sort(T* array, const int length, Comparator cmp) {
    if (length <= 1)
        return;

    int left_length = length/2;
    int right_length = length - left_length;
    merge_sort<T, Comparator>(array, left_length, cmp);
    merge_sort<T, Comparator>(array+left_length, right_length, cmp);
    T* temp = new T[length];
    merge<T, Comparator>(array, left_length,
                        array+left_length, right_length,
                        temp, cmp);
    std::copy(temp, temp+length, array);
    delete temp;
}

template <typename T, typename Comparator>
void traverse(T array[], int length, Comparator cmp) {
    int thickness = 0;
    int count = 0;
    int last_coordinate = -1;
    int distance = 0;
    merge_sort<T, Comparator>(array, length, cmp);
    for (int i = 0; i < length; i++) {
        T point = array[i];
        if (last_coordinate != -1) {
            distance = point.coordinate - last_coordinate;
        }
        last_coordinate = point.coordinate;
        if (thickness == 1) {
            count += distance;
        }
        thickness += point.change;
    }
    std::cout << count << std::endl;
}

int main() {
    int N;

    std::cin >> N;
    assertm(N <= 10000, "N must be 10000 or less");

    Line array[2*N];
    LineComparator cmp;
    for (int i = 0; i < N; i++) {
        int left = 0, right = 0;
        std::cin >> left >> right;
        assertm(left >= 0 && left <= 1000000000,
                "L must be between 0 and 1000000000");
        assertm(right >= 0 && right <= 1000000000,
                "R must be between 0 and 1000000000");
        assertm(left < right, "Line segment length must be positive");
        Line left_element{left, 1};
        array[i] = left_element;
        Line right_element{right, -1};
        array[i+N] = right_element;
    }
    traverse<Line, LineComparator>(array, 2*N, cmp);

    return 0;
}
