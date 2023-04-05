/*
3_2. Реализовать дек с динамическим зацикленным буфером.
Требования: Дек должен быть реализован в виде класса.
*/

#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

class Deque{
 public:
    Deque()
        : arr_size(1)
        , front(0)
        , back(0)
        , element_count(0) {
        arr = new int[arr_size]();
    }
    ~Deque() {
        delete arr;
    }
    void push_front(int value);
    int pop_front();
    void push_back(int value);
    int pop_back();

 private:
    int front;
    int back;
    int arr_size;
    int element_count;
    int* arr;
    void grow();
};

void Deque::grow() {
    int new_size = arr_size*2;
    int* new_arr = new int[new_size]();

    /* 
    std::copy нарушит порядок в деке, т. к. место добавится с правого конца массива
    Добавленные в это пространство элементы нарушат зацикленность
    Создание нового массива вручную позволяет переместить указатели в заранее известные позиции
    */
    for (int i = 0; i < element_count; i++) {
        new_arr[i] = arr[(front + i) % arr_size];
    }
    front = 0;
    back = element_count - 1;
    std::copy(new_arr, new_arr+element_count, arr);
    delete[] new_arr;
    arr_size = new_size;
}

void Deque::push_front(int value) {
    if (element_count == arr_size)
        grow();

    if (front == 0)
        front = arr_size - 1;
    else
        front--;

    arr[front] = value;
    element_count++;
}

int Deque::pop_front() {
    if (element_count == 0)
        return -1;

    int value = arr[front];
    element_count--;

    if (front == arr_size - 1)
        front = 0;
    else
        front++;

    return value;
}

void Deque::push_back(int value) {
    if (element_count == arr_size)
        grow();

    if (back == arr_size - 1)
        back = 0;
    else
        back++;

    arr[back] = value;
    element_count++;
}

int Deque::pop_back() {
    if (element_count == 0)
        return -1;

    int value = arr[back];
    element_count--;

    if (back == 0)
        back = arr_size - 1;
    else
        back--;

    return value;
}

int main() {
    int n;
    bool valid = true;

    Deque deque;

    std::cin >> n;
    assertm(n <= 1000000, "n must be lower than or equal to 1000000");

    for (int i = 0; i < n; i++) {
        int command = 0, value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 1: {
                deque.push_front(value);
                break;
            }
            case 2: {
                int expected = value;
                int actual = deque.pop_front();
                if (valid && expected != actual)
                    valid = false;
                break;
            }
            case 3: {
                deque.push_back(value);
                break;
            }
            case 4: {
                int expected = value;
                int actual = deque.pop_back();
                if (valid && expected != actual)
                    valid = false;
                break;
            }
        }
    }

    if (valid)
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
}
