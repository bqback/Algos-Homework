/*
2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n >> m. 
Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.
В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1]. 
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска, 
а потом уже в нем делать бинарный поиск.
*/

#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

int exp_search(int arr[], int start, int end, int value) {
    if (arr[start] == value)
        return start;

    int index = start + 1;
    while (index < end && arr[index] <= value)
        index *= 2;

    return index/2;
}

bool binary_search(int arr[], int start, int end, int value) {
    if (start <= end) {
        int middle = (start + end)/2;

        if (arr[middle] == value)
            return true;

        if (arr[middle] > value)
            return binary_search(arr, start, middle - 1, value);

        return binary_search(arr, middle + 1, end, value);
    }

    return false;
}

void overlap(int A[], int B[], int size_A, int size_B) {
    int left = 0;
    int right;

    for (int i = 0; i < size_B; i++) {
        left = exp_search(A, left, size_A, B[i]);
        right = std::min(left*2, size_A);
        if (binary_search(A, left, right, B[i]))
            std::cout << B[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int n;
    int m;

    std::cin >> n;
    assertm(n > 0, "Size of array A must be greater than 0");
    std::cin >> m;
    assertm(m > 0, "Size of array B must be greater than 0");
    assertm(n > m, "Size of array A must be greater than size of array B");

    int* A = new int[n];
    int* B = new int[m];

    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
        if (i > 1)
            assertm(A[i-1] <= A[i], "Array must be sorted in ascending order");
    }

    for (int j = 0; j < m; j++) {
        std::cin >> B[j];
        if (j > 1)
            assertm(B[j-1] <= B[j], "Array must be sorted in ascending order");
    }

    overlap(A, B, n, m);

    delete[] A;
    delete[] B;

    return 0;
}
