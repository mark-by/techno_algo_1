#include <iostream>
#include <algorithm>
#include <bitset>
#include <vector>

struct input_data {
    int size;
    unsigned long long * array;
    input_data() : size(0) {
        array = nullptr;
    }
    ~input_data() {
        delete [] array;
    }
};

template <class T>
int bin_partition(T * arr, int size, int digit_idx);

template <class T>
void bin_qsort(T * arr, int size, int sizeof_element_in_bits);

void input(input_data &i_data);

template <class T>
void output(T * array, int size);


int main() {
    input_data i_data;
    input(i_data);
    bin_qsort(&i_data.array[0], i_data.size, sizeof(long long) * 8);
    output(i_data.array, i_data.size);
    return 0;
}

template <class T>
int bin_partition(T * arr, int size, int digit_idx) {
    if ( size <= 1) {
        return 0;
    }
    int i = 0;
    int j = size - 1;
    while ( i <= j) {
        for (; !std::bitset<sizeof(T)*8>(arr[i])[digit_idx - 1] && i < size; i++);
        for (; j >= 0 && std::bitset<sizeof(T)*8>(arr[j])[digit_idx - 1]; j--);
        if (i < j) {
            std::swap(arr[i++], arr[j--]);
        }
    }
    return i;
}

template <class T>
void bin_qsort(T * arr, int size, int sizeof_element_in_bits) {
    if (size <= 1 || sizeof_element_in_bits < 1) {
        return;
    }
    int part = bin_partition(arr, size, sizeof_element_in_bits);
    bin_qsort(arr, part, sizeof_element_in_bits - 1);
    bin_qsort(arr + part, size - part, sizeof_element_in_bits - 1);
}

void input(input_data & i_data) {
    std::cin >> i_data.size;
    i_data.array = new unsigned long long [i_data.size];
    for (int i = 0; i < i_data.size; i++) {
        std::cin >> i_data.array[i];
    }
}

template <class T>
void output(T * array, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}
