#include <iostream>

typedef struct Point {
    int x;
    int y;
    Point(): x(0), y(0) {};
    Point(int x, int y): x(x), y(y) {};
} Point;

float calculate_area_of_trapeze(const Point *const p1, const Point *const p2) {
   return (float)((p1->y + p2->y)*(p1->x - p2->x))/2;
}

float calculate_area_of_poly(const Point *const points, const int size) {
    float result = 0;
    int i = 0;
    for (; i < size-1; i++) {
        result += calculate_area_of_trapeze(&(points[i]), &(points[i+1]));
    }

    result += calculate_area_of_trapeze(&(points[i]), points);
    
    return result;
}

int main() {
    int number_of_points;
    std::cin >> number_of_points;
    Point * points = new Point[number_of_points];
    for (int i=0; i < number_of_points; i++) {
        std::cin >> points[i].x >> points[i].y;
    }

    std::cout << calculate_area_of_poly(points, number_of_points) << std::endl;

    delete [] points;
    return 0;
}
