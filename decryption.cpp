#include "decryption.h"

#include <assert.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>

vector<string> loadFile(const string& filename) {
    ifstream file{filename};
    assert(file.good());
    string text;
    string line;
    vector<string> msg;
    getline(file, text);
    for (int i{0}; i < text.length(); i++) {
        line.push_back(text[i]);
        if (line.length() == BLOCK) {
            msg.push_back(line);
            line.clear();
        }
    }
    if (!line.empty()) {
        while (line.length() < BLOCK) {
            line.push_back(' ');
        }
        msg.push_back(line);
    }
    return msg;
}

void saveFile(const string& filename, const vector<string>& msg) {
    ofstream file{filename};
    assert(file.good());
    for (const string& line : msg) {
        file << line;
    }
}

string formatLine(const string& line, const vector<int>& sequence) {
#ifdef DEBUG
    set<int> test;
    for (int n : sequence) {
        test.insert(n);
    }
    assert(test.size() == sequence.size());
    assert(*test.begin() == 0);
    assert(*(--test.end()) == sequence.size() - 1);
#endif
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

vector<string> loadDict(const string& filename) {
    ifstream file{filename};
    assert(file.good());
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

    Sequence generator{BLOCK};
    const vector<vector<int>> everySeq = generator.get();
    vector<int> bestSeq;
    int tries = 0;
    double bestValue = -1;
    constexpr int maxWords = 10;
    constexpr double correctness = 80.0;

    for (const auto& seq : everySeq) {
        tries++;
        vector<string> msgTest = decrypt(msg, seq);
        vector<string> words = msgToWords(msgTest);
        double value = correctWordsPercent(words, dict, maxWords);
        if (value > bestValue) {
            bestValue = value;
            bestSeq = seq;
            if (bestValue >= correctness) {
                break;
            }
        }
    }

    if (PRINT) {
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