#include <iostream>
#include <vector>
#include <algorithm>

#define BEGIN 1 //event type когда человек может быть современником
#define END 0 //event type когда человек перестает быть современником
#define Y *10000
#define M *100
#define D *1

struct event {
    int date;
    bool type;
    event() : date(), type(BEGIN) {}
    event(int date, char type) : date(date), type(type) {}
};

template <class T>
struct default_comparator {
    bool operator() (const T& l, const T& r) {
        return l < r;
    }
};

struct event_comparator {
    bool operator() (const event & l, const event & r) {
        return l.date < r.date;
    }
};

void input(std::vector<event> & events);

template <class T, class Comparator = default_comparator<T>>
void merge(const T * first_arr, size_t first_size, const T * second_arr, size_t second_size, T * buffer, Comparator comp);

template <class T, class Comparator = default_comparator<T>>
void merge_sort( T * arr, size_t size, Comparator comp = Comparator());

class Counter {
public:
    Counter() : value(0), max_value(0) {}
    void inc();
    void dec();

    size_t value;
    size_t max_value;
};

int answer(event * events, size_t amount) {
    Counter counter;
    for (int i = 0; i < amount; i++) {
        if (events[i].type) {
            counter.inc();
        } else {
            counter.dec();
        }
    }

    return counter.max_value;
}

int main() {
    std::vector<event> events;
    input(events);
    merge_sort(&events[0], events.size(), event_comparator());
    std::cout << answer(&events[0], events.size());
    return 0;
}

template <class T, class Comparator>
void merge(const T * first_arr, size_t first_size, const T * second_arr, size_t second_size, T * buffer, Comparator comp) {
    size_t first_itr = 0, second_itr = 0, buffer_itr = 0;
    while (first_itr < first_size && second_itr < second_size) {
        if (comp(second_arr[second_itr], first_arr[first_itr])) {
            buffer[buffer_itr++] = second_arr[second_itr++];
        } else {
            buffer[buffer_itr++] = first_arr[first_itr++];
        }
    }
    if ( first_itr < first_size ) {
        std::copy(first_arr + first_itr, first_arr + first_size, buffer + buffer_itr);
    }
    if ( second_itr < second_size ) {
        std::copy(second_arr + second_itr, second_arr + second_size, buffer + buffer_itr);
    }
}

template <class T, class Comparator>
void merge_sort( T * arr, size_t size, Comparator comp) {
    if (size <= 1) {
        return;
    }
    size_t first_size = size / 2;
    size_t second_size = size - first_size;
    merge_sort( arr, first_size, comp);
    merge_sort( arr + first_size, second_size, comp);
    T * buffer = new T[size];
    merge(arr, first_size, arr + first_size, second_size, buffer, comp);
    std::copy(buffer, buffer + size, arr);
    delete [] buffer;
}

void add_events_to(std::vector<event> & events, int born_date, int death_date) {
    int date_18 = born_date + 18 Y;
    int date_80 = born_date + 80 Y;
    if (death_date <= date_18) {
        return;
    }
    event event_begin(date_18, BEGIN);
    event event_end(date_80, END);
    if (death_date < date_80) {
        event_end.date = death_date;
    }
    events.push_back(event_begin);
    events.push_back(event_end);
}

void input(std::vector<event> & events) {
    size_t amount;
    std::cin >> amount;
    int temp[6];
    for (size_t times = 0; times < amount; times++) {
        for (int cell = 0; cell < 6; cell++) {
            std::cin >> temp[cell];
        }
        add_events_to(events,
                      temp[0] D + temp[1] M + temp[2] Y,
                      temp[3] D + temp[4] M + temp[5] Y);
    }
}

void Counter::inc() {
    value++;
    if (value > max_value) {
        max_value = value;
    }
}

void Counter::dec() {
    value--;
}

