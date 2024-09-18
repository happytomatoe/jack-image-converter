#pragma once
#include <regex>
#include <string>
#include <vector>
#include "Rectangle.h"
using namespace std;

class ImageConverter {
public:
    static string convert(const string &filepath,
                          const bool ignore_checksums,
                          const bool debug, 
                          const bool export_size, 
                          const bool disable_grouping);

    static vector<Rectangle> findRectangles(const vector<vector<bool>>& image);  

private:
    static string convert_to_jack_code(const vector<vector<bool>> &img,  const bool export_size, const bool disable_grouping);

    static vector<vector<bool> >
    convert_to_binary_image(const std::vector<unsigned char> &image, unsigned w, unsigned h);

    static string printImage(const std::vector<unsigned char> &image, unsigned w, unsigned h);

    static void read(const string &filepath, bool ignore_checksums, std::vector<unsigned char> &image, unsigned &w,
                     unsigned &h);              
};
