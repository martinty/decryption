#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Sequence.h"

using namespace std;

//------------------------------------------------------------------------------'

// Columnar transposition
// Transposition block has 7 characters
constexpr int block = 7;
constexpr bool debug = true;
constexpr bool print = true;

vector<string> loadFile(string filename) {
    ifstream file{filename};
    if (!file) {
        runtime_error{"Couldn't open file: " + filename};
    }
    string text;
    getline(file, text);
    string line;
    vector<string> msg;
    for (int i{0}; i < text.length(); i++) {
        line.push_back(text[i]);
        if (line.length() == block) {
            msg.push_back(line);
            line.clear();
        }
    }
    if (!line.empty()) {
        while (line.length() < block) {
            line.push_back(' ');
        }
        msg.push_back(line);
    }
    return msg;
}

void saveFile(string filename, vector<string> msg) {
    ofstream file{filename};
    if (!file) {
        runtime_error{"Couldn't open file: " + filename};
    }
    for (const string& word : msg) {
        file << word;
    }
}

string formatLine(string line, vector<int> sequence) {
    if (debug) {
        set<int> test;
        for (int n : sequence) {
            test.insert(n);
        }
        if (test.size() != sequence.size() || *test.begin() != 0 ||
            *(--test.end()) != block - 1) {
            throw runtime_error{"Incorrect sequence!"};
        }
        if (line.length() != sequence.size()) {
            throw runtime_error{"Line and sequance is not the same size!"};
        }
    }
    string newLine = line;
    for (int i{0}; i < line.length(); i++) {
        newLine[i] = line[sequence[i]];
    }
    return newLine;
}

vector<string> decrypt(vector<string> msg, const vector<int>& sequence) {
    for (string& line : msg) {
        line = formatLine(line, sequence);
    }
    return msg;
}

vector<string> msgToWords(const vector<string>& msg) {
    string text;
    for (const string& line : msg) {
        text += line;
    }
    vector<string> words;
    string word;
    stringstream ss{text};
    while (getline(ss, word, ' ')) {
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    return words;
}

vector<string> loadDict(string filename) {
    ifstream file{filename};
    if (!file) {
        runtime_error{"Couldn't open file: " + filename};
    }
    vector<string> dict;
    string word;
    while (getline(file, word)) {
        dict.push_back(word);
    }
    return dict;
}

double correctWordsPercent(const vector<string>& words,
                           const vector<string>& dict, const int maxWords) {
    int correct = 0;
    int n = 0;
    for (const string& w : words) {
        for (const string& d : dict) {
            if (w[0] > d[0]) {
                continue;
            }
            if (w[0] < d[0]) {
                break;
            }
            if (w == d) {
                correct++;
                break;
            }
        }
        n++;
        if (n >= maxWords) {
            break;
        }
    }
    return 100.0 * (double)correct / (double)n;
}

vector<int> findCorrectSequence(const vector<string>& msg,
                                const vector<string>& dict) {
    // vector<int> correctSeq{4, 3, 5, 1, 6, 0, 2};

    Sequence generator{block};
    vector<vector<int>> everySeq = generator.get();

    vector<string> words;
    vector<string> msgTest;
    vector<int> bestSeq;
    int tries = 0;
    double bestValue = -1;

    for (const auto& seq : everySeq) {
        tries++;
        msgTest = decrypt(msg, seq);
        words = msgToWords(msgTest);
        double value = correctWordsPercent(words, dict, 10);
        if (value > bestValue) {
            bestValue = value;
            bestSeq = seq;
            if (bestValue > 80) {
                break;
            }
        }
    }

    if (print) {
        cout << fixed << setprecision(2);
        cout << setw(30) << "Sequences tried: " << setw(10) << tries << "\n"
             << setw(30) << "Correct enligsh in %: " << setw(10) << bestValue
             << "\n";
        string correctSeq;
        for (int n : bestSeq) {
            correctSeq += to_string(n);
        }
        cout << setw(30) << "Correct sequence: " << setw(10) << correctSeq
             << "\n";
    }
    return bestSeq;
}

int main() {
    try {
        vector<string> msg = loadFile("msg.txt");
        vector<string> american = loadDict("/usr/share/dict/american-english");

        auto startTime = chrono::steady_clock::now();

        // Work start
        vector<int> sequence = findCorrectSequence(msg, american);
        // Work done

        auto endTime = chrono::steady_clock::now();
        double duration = chrono::duration_cast<std::chrono::milliseconds>(
                              endTime - startTime)
                              .count() /
                          1000.0;

        if (print) {
            cout << fixed << setprecision(3);
            cout << setw(30) << "Elapsed time in seconds: " << setw(10)
                 << duration << "\n";
        }

        vector<string> msgDecrypted = decrypt(msg, sequence);
        saveFile("decrypted.txt", msgDecrypted);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}

//------------------------------------------------------------------------------
