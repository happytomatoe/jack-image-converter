#include <iostream>

// #include "converter/StringUtils.h"
using namespace std;
#include "converter/ImageConverter.hpp"
// #include <cpptrace/cpptrace.hpp>
// #include "clip.h"
// #include <CLI/CLI.hpp>

// bool yes_or_no_prompt(const string &prompt) {
//     cout << prompt << endl;
//     string answer;
//     while (true) {
//         getline(cin, answer);
//         if (answer == "y") {
//             return true;
//         } else if (answer == "n") {
//             return false;
//         } else {
//             cout << "Please enter y or n" << endl;
//         }
//     }
// }


int main(int argc, char *argv[]) {

    //get first agument which is input file
    string input_file = argv[1];
     const auto res = ImageConverter::convert(input_file, false, false, false, false);
     cout << res;
//     CLI::App app{"Convert black and white png to Jack code"};
//     argv = app.ensure_utf8(argv);

//     string input_file;
//     app.add_option("input", input_file, "input file path")
//             ->check(CLI::ExistingFile)->required();
    
//     bool debug = false, print_res = false, ignore_checksums = false, export_size=false, disable_grouping=false;
//     app.add_flag("-d,--debug", debug, "Debug mode");
//     app.add_flag("--ic,--ignore-checksums", ignore_checksums, "Ignore checksums when decompressing png");
//     app.add_flag("-o,--output", print_res, "Print result instead of copying to clipboard");
//     app.add_flag("-s,--export-size", export_size, "Export size of the image");
//     app.add_flag("--dg,--disable-grouping", disable_grouping, "Disable grouping pixels into lines or rectangles");

//     CLI11_PARSE(app, argc, argv);
//     if (debug) {
//         cpptrace::register_terminate_handler();
//     }
    // const auto res = ImageConverter::convert(input_file, ignore_checksums, debug, export_size, disable_grouping);
//     if (print_res) {
//         cout << res;
//     } else {
//         clip::set_text(res);
//         cout << "Copied to clipboard" << endl;
//     }
//     return 0;
}
