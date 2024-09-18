#pragma once
#include <iostream>
using namespace std;
struct Rectangle {
    int top_left_x, top_left_y;
    int bottom_right_x, bottom_right_y;

    // Constructor to initialize the rectangle's corners
    Rectangle(int min_x, int min_y, int max_x, int max_y)
        : top_left_x(min_x),
          top_left_y(min_y),
          bottom_right_x(max_x),
          bottom_right_y(max_y) {}

    // Overload the equality operator to compare two rectangles
    bool operator==(const Rectangle& other) const {
        return top_left_x == other.top_left_x &&
               top_left_y == other.top_left_y &&
               bottom_right_x == other.bottom_right_x &&
               bottom_right_y == other.bottom_right_y;
    }

    // Method to calculate the size (area) of the rectangle
    int size() const {
        int width = bottom_right_x - top_left_x + 1;
        int height = bottom_right_y - top_left_y + 1;
        return width * height;
    }
    friend ostream& operator<<(ostream& os, const Rectangle& rect) {
        os << "Rectangle(" << rect.top_left_x << ", " << rect.top_left_y << ", "
           << rect.bottom_right_x << ", " << rect.bottom_right_y << ")";
        return os;
    }

    bool is_line() const {
        return (top_left_x == bottom_right_x || top_left_y == bottom_right_y) &&
               size() > 1;
    }

    string convert_to_jack_code() {
        string x0, y0, x1, y1;
        if (top_left_x != 0) {
            x0 = "+" + to_string(top_left_x);
        }
        // same for others
        if (top_left_y != 0) {
            y0 = "+" + to_string(top_left_y);
        }
        if (bottom_right_x != 0) {
            x1 = "+" + to_string(bottom_right_x);
        }
        if (bottom_right_y != 0) {
            y1 = "+" + to_string(bottom_right_y);
        }

        if (size() == 1) return format("do Screen.drawPixel(x{},y{});\n", x0, y0);
        const string func_name = is_line() ? "Screen.drawLine" : "Screen.drawRectangle";
        return format("do {}(x{},y{},x{},y{});\n", func_name, x0, y0, x1, y1);
    }
};
