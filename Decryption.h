#include <assert.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>

#include "Dictionary.h"
#include "Sequence.h"

using namespace std;

// Columnar transposition
// Transposition BLOCK has 7 characters
constexpr int BLOCK = 7;
constexpr bool PRINT = true;

vector<string> loadFile(const string& filename);
void saveFile(const string& filename, const vector<string>& msg);
string formatLine(const string& line, const vector<int>& sequence);
vector<string> decrypt(vector<string> msg, const vector<int>& sequence);
vector<string> msgToWords(const vector<string>& msg);
vector<string> loadDict(const string& filename);
double correctWordsPercent(const vector<string>& words,
                           const vector<string>& dict, const int maxWords);
vector<int> findCorrectSequence(const vector<string>& msg,
                                const vector<string>& dict);

class Decryption {
   private:
    const int block;
    Dictionary dict;
    Sequence seqs;
    vector<string> originalMsg;
    vector<string> solvedMsg;
    vector<int> bestSeq;
    double bestValue;
    int tries;

    string formatLine(const string& line, const vector<int>& seq) {
#ifdef DEBUG
        set<int> test;
        for (int n : seq) {
            test.insert(n);
        }
        assert(test.size() == seq.size());
        assert(*test.begin() == 0);
        assert(*(--test.end()) == seq.size() - 1);
#endif
        string newLine = line;
        for (int i{0}; i < line.length(); i++) {
            newLine[i] = line[seq[i]];
        }
        return newLine;
    }
    vector<string> decrypt(vector<string> msg, const vector<int>& seq) {
        for (string& line : msg) {
            line = formatLine(line, seq);
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
    double correctWordsPercent(const vector<string>& words,
                               const int maxWords) {
        int correct = 0;
        int n = 0;
        for (const string& w : words) {
            if (dict.isInDictionary(w)) {
                correct++;
            }
            n++;
            if (n >= maxWords) {
                break;
            }
        }
        return 100.0 * (double)correct / (double)n;
    }
    void findCorrectSequence(const int maxWords, const double correctness) {
        // vector<int> correctSeq{4, 3, 5, 1, 6, 0, 2};
        tries = 0;
        bestValue = -1;

        for (const auto& seq : seqs.get()) {
            tries++;
            vector<string> msgTest = decrypt(originalMsg, seq);
            vector<string> words = msgToWords(msgTest);
            double value = correctWordsPercent(words, maxWords);
            if (value > bestValue) {
                bestValue = value;
                bestSeq = seq;
                if (bestValue >= correctness) {
                    break;
                }
            }
        }
    }

   public:
    Decryption(int block = 7) : block{block}, seqs{block} {};
    ~Decryption() = default;
    void addDict(const string& filename) { dict.load(filename); }
    void loadMsg(const string& filename) {
        ifstream file{filename};
        assert(file.good());
        string text;
        string line;
        getline(file, text);
        for (int i{0}; i < text.length(); i++) {
            line.push_back(text[i]);
            if (line.length() == block) {
                originalMsg.push_back(line);
                line.clear();
            }
        }
        if (!line.empty()) {
            while (line.length() < block) {
                line.push_back(' ');
            }
            originalMsg.push_back(line);
        }
    }
    void saveMsg(const string& filename) const {
        ofstream file{filename};
        assert(file.good());
        for (const string& line : solvedMsg) {
            file << line;
        }
    }

    void solve() {
        constexpr int maxWords = 10;
        constexpr double correctness = 80.0;
        findCorrectSequence(maxWords, correctness);
        solvedMsg = decrypt(originalMsg, bestSeq);

        if (PRINT) {
            cout << fixed << setprecision(2);
            cout << setw(30) << "Sequences tried: " << setw(10) << tries << "\n"
                 << setw(30) << "Correct enligsh in %: " << setw(10)
                 << bestValue << "\n";
            string correctSeq;
            for (int n : bestSeq) {
                correctSeq += to_string(n);
            }
            cout << setw(30) << "Correct sequence: " << setw(10) << correctSeq
                 << "\n";
        }
    }
};
