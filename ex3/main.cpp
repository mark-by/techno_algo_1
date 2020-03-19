#include <iostream>
#include <stdexcept>
#include <algorithm>

#define INITIAL_CAPACITY 7

template <class T>
class DynamicArray {
    public:
        DynamicArray(): size(0), capacity(INITIAL_CAPACITY) {buffer = new T[capacity];}
        ~DynamicArray() {delete [] buffer;}
        DynamicArray(const DynamicArray& other);

        void add(T element);
        size_t get_size() const { return size;}
        size_t get_capacity() const {return capacity;}
        T& operator[] (int index);

    protected:
        size_t size;
        size_t capacity;
        T* buffer;

        void grow();

};

template <class T>
class Queue: public DynamicArray<T> {
    public:
        explicit Queue();
        Queue(const Queue&);
        Queue(Queue&&) = delete;
        Queue& operator= (const Queue&) = delete;
        Queue& operator= (Queue&&) = delete;

        void push(T element);
        T pop();
        bool is_empty() const {return head == tail;}

    private:
        using DynamicArray<T>::capacity;
        using DynamicArray<T>::size;
        using DynamicArray<T>::buffer;
        using DynamicArray<T>::grow;
        int head; //индекс первого элемент очереди
        int tail; //индекс следующего после последнего
};

template <class T>
void test(Queue <T> queue);

/*===============MAIN==========*/

int main() {
    Queue <int> queue;
    test(queue);
    return 0;
}

/*===========================*/

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    size = other.buffer;
    capacity = other.capacity;
    buffer = new T[capacity];
    std::copy(other.buffer, other.buffer + capacity, buffer);
}

template <class T>
void DynamicArray<T>::add(T element) {
    if (size != capacity) {
        buffer[size] = element;
        size++;
    } else {
        grow();
        add(element);
    }
}

template <class T>
void DynamicArray<T>::grow() {
    T* temp_buffer = new T[capacity * 2];
    std::copy(buffer, buffer + capacity, temp_buffer);
    delete [] buffer;
    buffer = temp_buffer;
    capacity *= 2;
}

template <class T>
T& DynamicArray<T>::operator[](int index) {
    return buffer[index];
}

template <class T>
Queue<T>::Queue(): head(0), tail(0) {}

template <class T>
void Queue<T>::push(T element) {
    if ((tail + 1) % capacity == head) {
        grow();
        if (tail < head) {
            std::copy(buffer, buffer + tail, buffer + capacity/2);
            tail = head + size;
        }
    }
    buffer[tail] = element;
    tail = (tail + 1) % capacity;
    size++;
}

template <class T>
T Queue<T>::pop() {
    if (head == tail) {
        return -1;
    }
    T result = buffer[head];
    head = (head + 1) % capacity;
    size--;
    return result;
}

template <class T>
Queue<T>::Queue(const Queue& other) {
    head = other.head;
    tail = other.tail;
}

template <class T>
void test(Queue <T> queue) {
    int in_a, in_b, n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> in_a >> in_b;
        switch (in_a) {
            case 2:
                if (queue.pop() != in_b) {
                    std::cout << "NO\n" << std::endl;
                    return;
                }
                break;
            case 3:
                queue.push(in_b);
                break;
        }
    }
    std::cout << "YES\n" << std::endl;
}
