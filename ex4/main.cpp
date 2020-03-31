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
    T& last() { return buffer[size - 1];}
    bool is_empty() const {return size == 0; }
    T& operator[] (int index);

protected:
    size_t size;
    size_t capacity;
    T* buffer;

    void grow();
};

template <class T>
struct DefaultComparator {
    bool operator() (const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class Comparator = DefaultComparator<T>>
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
    Comparator comp = Comparator();
    bool lt(const T& l, const T& r);
    bool gt(const T& l, const T& r);
    bool eq(const T&, const T& r);
    using DynamicArray<T>::pop_back;
    using DynamicArray<T>::capacity;
    using DynamicArray<T>::size;
    using DynamicArray<T>::buffer;
    using DynamicArray<T>::grow;
    using DynamicArray<T>::operator[];
    using DynamicArray<T>::last;
};

struct input_data {
    int size;
    DynamicArray<int> fruits_mass;
    int carrying;
    input_data() {
        size = 0;
        carrying = 0;
    }
};

void input(input_data &data);

int count_attemps(const DynamicArray<int>& fruits, int carrying);


// MAIN

int main(int args, char ** argv) {
    input_data i_data;
    input(i_data);
    std::cout << count_attemps(i_data.fruits_mass, i_data.carrying) << std::endl;
    return 0;
}

// END MAIN


int count_attemps(const DynamicArray<int>& fruits, int carrying) {
    Heap<int> basket(fruits); // корзина
    DynamicArray<int> pockets; // карманы
    int attemps = 0; // количество попыток
    for (; !basket.is_empty(); attemps++) {
        int curr_carrying = 0;
        while( !basket.is_empty() && (curr_carrying + basket.top()) <= carrying ) {
            curr_carrying += basket.top();
            pockets.add(basket.pop());
        }
        while( !pockets.is_empty() ) {
            if (pockets.last() != 1) {
                basket.add(pockets.pop_back() / 2);
            } else {
                pockets.pop_back();
            }
        }
    }
    return attemps;
}

// HEAP REALIZATION

template <class T, class Comparator>
Heap<T, Comparator>::Heap(const T * array, size_t arr_size) : DynamicArray<T>(array, arr_size) {
    sift_down();
}

template <class T, class Comparator>
Heap<T, Comparator>::Heap(const DynamicArray<T>& array) : DynamicArray<T>(array) {
    sift_down();
}

template <class T, class Comparator>
bool Heap<T, Comparator>::lt(const T& l, const T& r) {
    return comp(l, r);
}

template <class T, class Comparator>
bool Heap<T, Comparator>::eq(const T& l, const T& r) {
    return (!comp(l, r) && !comp(r, l));
}

template <class T, class Comparator>
bool Heap<T, Comparator>::gt(const T& l, const T& r) {
    return comp(r, l);
}

template <class T, class Comparator>
void Heap<T, Comparator>::add(const T& element) {
    DynamicArray<T>::add(element);
    sift_up();
}

template <class T, class Comparator>
void Heap<T, Comparator>::sift_down(size_t index) {
    size_t left = index * 2 + 1;
    size_t right = index * 2 + 2;
    size_t largest = index;
    if (left < size && gt(buffer[left], buffer[largest])) {
        largest = left;
    }
    if (right < size && gt(buffer[right], buffer[largest])) {
        largest = right;
    }

    if (largest != index) {
       std::swap(buffer[index], buffer[largest]);
       sift_down(largest);
    }
}

template <class T, class Comparator>
void Heap<T, Comparator>::sift_down() {
    for (int i = size / 2 - 1; i >= 0; i--) {
        sift_down(i);
    }
}

template<class T, class Comparator>
void Heap<T, Comparator>::sift_up() {
    size_t index = size - 1;
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (!gt(buffer[index], buffer[parent])) {
            return;
        }
        std::swap(buffer[index], buffer[parent]);
        index = parent;
    }
}

template <class T, class Comparator>
T Heap<T, Comparator>::pop() {
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

template <class T, class Comparator>
const T& Heap<T, Comparator>::top() const {
    if (is_empty()) {
        throw std::runtime_error("Heap is empty");
    }

    return buffer[0];
}

// END HEAP REALIZATION


// DYNAMIC ARRAY REALIZATION

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    size = other.size;
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
    std::cin >> data.size;
    int temp;
    for (int i=0; i < data.size; i++ ) {
        std::cin >> temp;
        data.fruits_mass.add(temp);
    }
    std::cin >> data.carrying;
}

