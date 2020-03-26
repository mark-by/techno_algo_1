#include <iostream>
#include <stdexcept>
#include <algorithm>

#define INITIAL_CAPACITY 7

template <class T>
class DynamicArray {
public:
    DynamicArray(): size(0), capacity(INITIAL_CAPACITY) {buffer = new T[capacity];}
    DynamicArray(const T * array, size_t size);
    virtual ~DynamicArray() {delete [] buffer;}
    DynamicArray(const DynamicArray& other);

    virtual void add(const T& element);
    size_t get_size() const { return size;}
    size_t get_capacity() const {return capacity;}
    T pop_back();
    T pop_first();
    bool is_empty() const {return size == 0; }
    T& operator[] (int index);

protected:
    size_t size;
    size_t capacity;
    T* buffer;

    void grow();
};

template <class T>
class Heap : public DynamicArray<T> {
public:
    explicit Heap() = default;
    explicit Heap(const DynamicArray<T>& array);
    Heap(const T * arr, size_t size);
    Heap(Heap&&) = delete;
    Heap& operator= (const Heap&) = delete;
    Heap& operator= (Heap&&) = delete;

    using DynamicArray<T>::is_empty;
    void add(const T& element) override;
    T pop();
    const T& top() const;

private:
    void sift_down(size_t i);
    void sift_down();
    void sift_up();
    using DynamicArray<T>::pop_back;
    using DynamicArray<T>::capacity;
    using DynamicArray<T>::size;
    using DynamicArray<T>::buffer;
    using DynamicArray<T>::grow;
    using DynamicArray<T>::operator[];
};

struct input_data {
    int amount;
    DynamicArray<int> fruct_mass;
    int carrying;
    input_data() {
        amount = 0;
        carrying = 0;
    }
};

void input(input_data &data);


// MAIN

int main(int args, char ** argv) {
//    input_data i_data;
//    input(i_data);
    int array[] = {5,6,8,3,6,3,8,4,5,6};
    Heap<int> heap(array, 10);
    for (int i = 0; i < 10; i++) {
        std::cout << heap.pop() << std::endl;
    }
    return 0;
}

// END MAIN


// HEAP REALIZATION

template <class T>
Heap<T>::Heap(const T * array, size_t arr_size) : DynamicArray<T>(array, arr_size) {
    sift_down();
}

template <class T>
Heap<T>::Heap(const DynamicArray<T>& array) : DynamicArray<T>(array) {
    sift_down();
}

template <class T>
void Heap<T>::add(const T& element) {
    DynamicArray<T>::add(element);
    sift_up();
}

template <class T>
void Heap<T>::sift_down(size_t index) {
    size_t left = index * 2 + 1;
    size_t right = index * 2 + 2;
    size_t largest = index;
    if (left < size && buffer[left] > buffer[largest]) {
        largest = left;
    }
    if (right < size && buffer[right] > buffer[largest]) {
        largest = right;
    }

    if (largest != index) {
       std::swap(buffer[index], buffer[largest]);
       sift_down(largest);
    }
}

template <class T>
void Heap<T>::sift_down() {
    for (int i = size / 2 - 1; i >= 0; i--) {
        sift_down(i);
    }
}

template<class T>
void Heap<T>::sift_up() {
    size_t index = size - 1;
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (buffer[index] <= buffer[parent]) {
            return;
        }
        std::swap(buffer[index], buffer[parent]);
        index = parent;
    }
}

template <class T>
T Heap<T>::pop() {
    if (is_empty()) {
        throw std::runtime_error("Heap is empty");
    }

    T result = buffer[0];
    buffer[0] = pop_back();

    if (!is_empty()) {
        sift_down(0);
    }
    return result;
}

template <class T>
const T& Heap<T>::top() const {
    if (is_empty()) {
        throw std::runtime_error("Heap is empty");
    }

    return buffer[0];
}

// END HEAP REALIZATION


// DYNAMIC ARRAY REALIZATION

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    size = other.buffer;
    capacity = other.capacity;
    buffer = new T[capacity];
    std::copy(other.buffer, other.buffer + capacity, buffer);
}

template <class T>
void DynamicArray<T>::add(const T& element) {
    if (size != capacity) {
        buffer[size] = element;
        size++;
    } else {
        grow();
        add(element);
    }
}

template <class T>
DynamicArray<T>::DynamicArray(const T * array, size_t arr_size) : DynamicArray() {
    if (!array) {
        throw std::invalid_argument("Array is nullptr");
    }
    for (int i = 0; i < arr_size; i++) {
        add(array[i]);
    }
}

template <class T>
T DynamicArray<T>::pop_back() {
    if (is_empty()) {
        throw std::runtime_error("Heap is empty");
    }
    size--;
    return buffer[size];
}

template <class T>
T DynamicArray<T>::pop_first() {
    if (is_empty()) {
        throw std::runtime_error("Heap is empty");
    }

    T result = buffer[0];
    size--;
    for (int i = 0; i < size; i++) {
        std::swap(buffer[i], buffer[i+1]);
    }
    return result;
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
    if (index < 0 || index >= size){
        throw std::out_of_range("Out of range");
    }
    return buffer[index];
}

// END DYNAMIC ARRAY REALIZATION

void input(input_data &data) {
    std::cin >> data.amount;
    int temp;
    for (int i=0; i < data.amount; i++ ) {
        std::cin >> temp;
        data.fruct_mass.add(temp);
    }
    std::cin >> data.carrying;
}

