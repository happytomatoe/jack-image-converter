#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

#include "converter/ImageConverter.hpp"
#include "converter/StringUtils.h"

using namespace std;

// trim from start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

string normalize(string str) {
    set empty_line = {'\n', ' ', '\t'};
    auto strings = StringUtils::split(str, '\n');
    string res;
    for (auto& line : strings) {
        ltrim(line);
        rtrim(line);
        if (set line_distinct_chars(line.begin(), line.end());
            !ranges::includes(empty_line, line_distinct_chars)) {
            res += line + "\n";
        }
    }
    return res;
}

TEST(ConverterTest, BasicTest) {
    string expected = R"(
        method void draw(int x, int y) {
           do Screen.drawPixel(x,y);
           do Screen.drawPixel(x+1,y+1);
           do Screen.drawPixel(x+2,y+2);
           do Screen.drawPixel(x+3,y+3);
           do Screen.drawPixel(x+4,y+4);
           return;
        }
        )";
    auto res = ImageConverter::convert("data/test.png", true, true, false, false);
    auto n1 = normalize(res);
    auto n2 = normalize(expected);
    cout << "Actual: " << endl
         << n1 << endl
         << "Expected: " << endl
         << n2 << endl;
    ASSERT_EQ(n1, n2);
}

TEST(ConverterTest, ShapesTest) {
    string expected = R"(
        method void draw(int x, int y) {
            do Screen.drawPixel(x,y);
            do Screen.drawRectangle(x+2,y,x+3,y+1);
            do Screen.drawPixel(x+1,y+1);
            do Screen.drawPixel(x,y+2);
            do Screen.drawRectangle(x+3,y+2,x+4,y+3);
            do Screen.drawLine(x,y+4,x+2,y+4);
            return;
        }
        )";
    auto res = ImageConverter::convert("data/shapes.png", true, true, false, false);
    auto n1 = normalize(res);
    auto n2 = normalize(expected);
    cout << "Actual: " << endl
         << n1 << endl
         << "Expected: " << endl
         << n2 << endl;
    ASSERT_EQ(n1, n2);
}

TEST(ConverterTest, FindRectangles) {
    // Example binary image (true = foreground, false = background)
    vector<vector<bool>> image = {{true, true, false, false, true},
                                  {true, false, false, false, true},
                                  {true, true, true, false, false},
                                  {false, true, true, false, false}};
    vector<Rectangle> expected = {
        Rectangle{2, 0, 3, 1},
        Rectangle{1, 1, 1, 1},
        Rectangle{3, 2, 4, 3},
        Rectangle{0, 3, 0, 3},

    };

    // Find the rectangles
    vector<Rectangle> rectangles = ImageConverter::findRectangles(image);
    ASSERT_EQ(rectangles, expected);
}
