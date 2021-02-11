#pragma once

#include <assert.h>

#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Dictionary {
   private:
    vector<string> dict;
    map<char, int> indexMap;
    string filename;
    int size = 0;

   public:
    Dictionary() = default;
    ~Dictionary() = default;
    Dictionary(string f) { load(f); }
    void load(string f) {
        dict.clear();
        indexMap.clear();
        filename = f;
        ifstream file{filename};
        assert(file.good());
        string word;
        char ch = static_cast<char>(NULL);
        int index = 0;
        while (getline(file, word)) {
            dict.push_back(word);
            if (word[0] > ch) {
                ch = word[0];
                indexMap[ch] = index;
            }
            index++;
        }
        size = dict.size();
    }
    bool isInDictionary(const string& word) const {
        const char first = word[0];
        if (indexMap.find(first) == indexMap.end()) {
            return false;
        }
        int i = indexMap.at(first);
        for (; i < size; i++) {
            const string& d = dict.at(i);
            if (first < d[0]) {
                return false;
            }
            if (word == d) {
                return true;
            }
        }
        return false;
    }
};
