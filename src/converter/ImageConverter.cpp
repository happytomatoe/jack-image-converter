#include "ImageConverter.hpp"

#include <lodepng.h>

#include <fstream>

#include "StringUtils.h"

string ImageConverter::convert(const string &filepath,
                               const bool ignore_checksums, const bool debug,
                               const bool export_size,
                               const bool disable_grouping) {
    std::vector<unsigned char> image;
    unsigned w;
    unsigned h;
    read(filepath, ignore_checksums, image, w, h);
    const auto img = convert_to_binary_image(image, w, h);
    if (debug) {
        std::cout << "Width: " << w << std::endl;
        std::cout << "Height: " << h << std::endl;
        std::cout << "Num pixels: " << w * h << std::endl;

        if (w > 0 && h > 0) {
            std::cout << "Top left pixel color:"
                      << " r: " << (int)image[0] << " g: " << (int)image[1]
                      << " b: " << (int)image[2] << " a: " << (int)image[3]
                      << std::endl;
        }
        cout << "Image lightness" << endl;
        cout << printImage(image, w, h) << endl;
        cout << "Binary image" << endl;
        StringUtils::print(img);
    }
    return convert_to_jack_code(img, export_size, disable_grouping);
}

string ImageConverter::convert_to_jack_code(const vector<vector<bool>> &img,
                                            const bool export_size,
                                            const bool disable_grouping) {
    string header;
    if (export_size) {
        header = format(R"(
            |field int imageW, imageH;
            |let imageW = {};
            |let imageH = {};
        )",
                        img[0].size(), img.size());
    }
    header += R"(|method void draw(int x, int y) {
)";
    string res = header;
    if (disable_grouping) {
        for (int i = 0; i < img.size(); i++) {
            auto row = img[i];
            for (int j = 0; j < row.size(); j++) {
                if (!row[j]) {
                    auto jStr = j > 0 ? format("+{}", j) : "";
                    auto iStr = i > 0 ? format("+{}", i) : "";
                    res += format("|   do Screen.drawPixel(x{},y{});\n", jStr, iStr);
                }
            }
        }
    } else {
        for (auto r : findRectangles(img)) {
            res += +"|" + r.convert_to_jack_code();
        }
    }
    auto footer = R"(|   return;
        |}
    )";
    return StringUtils::stripMargin(res + footer);
}

vector<vector<bool>> ImageConverter::convert_to_binary_image(
    const std::vector<unsigned char> &image, unsigned w, unsigned h) {
    if (w <= 0 || h <= 0) {
        throw runtime_error("Invalid image size: " + to_string(w) + "x" +
                            to_string(h));
    }
    vector res(h, vector(w, false));
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w * 4; x += 4) {
            if (image[y * w * 4 + x] > 224) res[y][x / 4] = true;
        }
    }
    return res;
}

string ImageConverter::printImage(const std::vector<unsigned char> &image,
                                  unsigned w, unsigned h) {
    if (w <= 0 || h <= 0) {
        throw runtime_error("Invalid image size: " + to_string(w) + "x" +
                            to_string(h));
    }
    string res;
    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w * 4; x += 4) {
            res += to_string(image[y * w * 4 + x]) + ",";
        }
        res += "\n";
    }
    return res;
}

void ImageConverter::read(const string &filepath, bool ignore_checksums,
                          std::vector<unsigned char> &image, unsigned &w,
                          unsigned &h) {
    ifstream f(filepath.c_str());
    if (!f.good()) {
        throw runtime_error("File not found: " + filepath);
    }

    std::vector<unsigned char> buffer;

    lodepng::load_file(buffer,
                       filepath);  // load the image file with given filename

    lodepng::State state;
    if (ignore_checksums) {
        state.decoder.ignore_crc = 1;
        state.decoder.zlibsettings.ignore_adler32 = 1;
    }

    unsigned error = decode(image, w, h, state, buffer);

    if (error) {
        throw runtime_error("decoder error " + to_string(error) + ": " +
                            lodepng_error_text(error));
    }
}

vector<Rectangle> ImageConverter::findRectangles(
    const vector<vector<bool>> &image) {
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<Rectangle> rectangles;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!image[i][j] && !visited[i][j]) {
                // Start a new rectangle
                int min_x = j, min_y = i;
                int max_x = j, max_y = i;

                // Expand to the right
                while (max_x + 1 < cols && !image[min_y][max_x + 1] &&
                       !visited[min_y][max_x + 1]) {
                    ++max_x;
                }

                // Expand downward
                bool can_expand_down = true;
                while (can_expand_down && max_y + 1 < rows) {
                    for (int x = min_x; x <= max_x; ++x) {
                        if (image[max_y + 1][x] || visited[max_y + 1][x]) {
                            can_expand_down = false;
                            break;
                        }
                    }
                    if (can_expand_down) {
                        ++max_y;
                    }
                }

                // Mark the rectangle as visited
                for (int y = min_y; y <= max_y; ++y) {
                    for (int x = min_x; x <= max_x; ++x) {
                        visited[y][x] = true;
                    }
                }

                // Store the rectangle
                rectangles.emplace_back(min_x, min_y, max_x, max_y);
            }
        }
    }

    return rectangles;
}
