#include <iostream>
#include <stdio.h>

template <class T>
class Queue {
    public:
        explicit Queue(int size); 
        ~Queue() {delete[] buffer;}
        Queue(const Queue<T>&); 
        Queue(Queue<T>&&) = delete;
        Queue& operator= (const Queue&) = delete;
        Queue& operator= (Queue&&) = delete;
        
        void push(T element);
        T pop();
        void realloc();

        bool is_empty() const {return head == tail;}

private:
        T* buffer;
        int buffer_size;
        int head; //индекс первого элемент очереди
        int tail; //индекс следующего после последнего
};

template <class T>
void test(Queue <T> queue);

/*===============MAIN==========*/

int main() {
    Queue <int> q(8);
    test(q);
    return 0;
}

/*===========================*/


template <class T>
Queue<T>::Queue(int size):
    buffer_size(size),
    head(0),
    tail(0) {
        buffer = new T[buffer_size];
    }

template <class T>
void Queue<T>::push(T element) {
    if ((tail + 1) % buffer_size == head) {
        realloc();
    }
    buffer[tail] = element;
    tail = (tail + 1) % buffer_size;
}

template <class T>
T Queue<T>::pop() {
    if (head == tail) {
        return -1;
    }
    T result = buffer[head];
    head = (head + 1) % buffer_size;
    return result;
}

template <class T>
void Queue<T>::realloc() {
    T * temp = new T[buffer_size * 2];
    int i = 0;
    for (; !is_empty(); i++) {
        temp[i] = pop();
    }
    head = 0;
    tail = i;
    delete [] buffer;
    buffer = temp;
    buffer_size *= 2;
}

template <class T>
Queue<T>::Queue(const Queue<T>& other) {
    buffer_size = other.buffer_size;
    buffer = new T[buffer_size];
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = other.buffer[i];
    }
    head = other.head;
    tail = other.tail;
}

template <class T>
void test(Queue <T> queue) {
    int in_a, in_b, n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &in_a, &in_b);
        switch (in_a) {
            case 2:
                if (queue.pop() != in_b) {
                    printf("NO\n");
                    return;
                }
                break;
            case 3:
                queue.push(in_b);
                break;
        }
    }
    printf("YES\n");
}
