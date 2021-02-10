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

// Columnar transposition
// Transposition BLOCK has 7 characters
constexpr int BLOCK = 7;
constexpr bool DEBUG = false;
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
