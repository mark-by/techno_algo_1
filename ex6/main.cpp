#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>

struct input_data {
    size_t size;
    size_t index;
    int * array;
    input_data() : size(0), index(0) {
        array = nullptr;
    }
    ~input_data() {
        delete array;
    }
};

template <class T>
struct default_comparator {
    bool operator() (const T& l, const T& r){
        return l < r;
    }
};

void input(struct input_data & i_data);

unsigned long random_ul(unsigned long l, unsigned long r);

template <class T, class Comparator = default_comparator<T>>
size_t partition(T* array, size_t l, size_t r, Comparator comp = Comparator());

template <class T, class Comparator = default_comparator<T>>
T kth_element(T* arr, size_t size, size_t k, Comparator comp = Comparator());

int main() {
    input_data i_data;
    input(i_data);
    std::cout << kth_element(i_data.array, i_data.size, i_data.index) << std::endl;
    return 0;
}

unsigned long random_ul(unsigned long l, unsigned long r) {
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_int_distribution<unsigned long> distribution(l, r);
    return distribution(dev);
}

template <class T, class Comparator>
size_t partition(T* array, size_t l, size_t r, Comparator comp) {
    if (l == r - 1) {
        return l;
    }
    size_t pivot_idx = random_ul(l, r - 1);
    T pivot = array[pivot_idx];

    std::swap(array[pivot_idx], array[r - 1]);

    size_t i = l;
    size_t j = l;
    while ( j != r - 1) {
       if (comp(pivot, array[j])) {
           j++;
       } else {
           std::swap(array[i], array[j]);
           i++;
           j++;
       }
    }
    std::swap(array[i], array[r-1]);
    return i;
}

template <class T, class Comparator>
T kth_element(T* arr, size_t size, size_t k, Comparator comp) {
    size_t l = 0;
    size_t r = size;

    while (l < r) {
        size_t m = partition(arr, l, r, comp);
        if (m == k) {
            return arr[k];
        }
        if (m < k) {
            l = m + 1;
        } else {
            r = m;
        }
    }

}

void input(struct input_data & i_data) {
   std::cin >> i_data.size >> i_data.index;
   i_data.array = new int[i_data.size];
   if (!i_data.array) {
       throw std::bad_alloc();
   }
   for (int i = 0; i < i_data.size; i++) {
       std::cin >> i_data.array[i];
   }
}
