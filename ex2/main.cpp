#include <iostream>
#include <stdio.h>

void find_range(int * array, int size, int *first, int *last) {
    //убывающий массив
    if (array[0] > array[1]) {
        *first = 0;
        *last = 0;
        return;
    }

    //возрастающий массив
    if (array[size - 2] < array[size - 1]) {
        *first = size - 1;
        *last = size - 1;
        return;
    }

    *first = 0;
    *last = 1;
    while (*last < size - 1) {
        //прошли m
        if (array[*last - 1] < array[*last]) {
            *last *= 2;
        } else {
            *first = *last / 2;
            return;
        }
    }
    //случается, если натыкаемся на last >= m && 2last > size - 1 
    if (*first == 0) {
        *first = *last / 2;
        *last = size - 1;
    }
}

int binary_search(const int* arr, int first, int last) {
    while (first < last) {
        int mid = (first + last) / 2;
        if (arr[mid] > arr[mid+1]) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    return first;
}

int * create_array(int * size) {
    std::cin >> *size;
    int * array = new int[*size];
    for (int i = 0; i < *size; i++) {
        scanf("%d", &(array[i]));
    }
    return array;
}

int main() {
    int size, first, last;
    int * array = create_array(&size);
    find_range(array, size, &first, &last);
    if (first != last) {
        std::cout << binary_search(array, first, last) << std::endl;
    } else {
        std::cout << last << std::endl;
    }
    return 0;
}
