#pragma once
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class StringUtils {
public:
    static vector<string> split(const string &str, char delimeter) {
        auto result = vector<string>{};
        stringstream ss(str);
        string line;
        while (getline(ss, line, delimeter)) {
            result.push_back(line);
        }
        return result;
    }

    static string stripMargin(const string &s) {
        const auto lines = split(s, '\n');
        string res;
        for (auto &line: lines) {
            int delimiterPos = 0;
            for (int i = 0; i < line.length(); ++i) {
                auto c = line[i];
                if (c == ' ' || c == '\t') {
                    continue;
                } else if (c == '|') {
                    delimiterPos = i + 1;
                    break;
                } else {
                    break;
                }
            }
            res += line.substr(delimiterPos) + "\n";
        }
        return res;
    }



    static void print(const vector<vector<bool> > &pixels) {
        for (const auto &pixel: pixels) {
            for (const auto &j: pixel) {
                cout << (j ? "1" : "0");
            }
            cout << endl;
        }
    };
};
