#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "decryption.h"

using namespace std;

int main() {
    try {
        const vector<string> msg = loadFile("msg.txt");
        const vector<string> american =
            loadDict("/usr/share/dict/american-english");

        const auto startTime = chrono::steady_clock::now();

        // Work start
        const vector<int> sequence = findCorrectSequence(msg, american);
        // Work done

        const auto endTime = chrono::steady_clock::now();
        const auto diff =
            chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        const double duration = diff.count() / 1000.0;

        if (PRINT) {
            cout << fixed << setprecision(3);
            cout << setw(30) << "Elapsed time in seconds: " << setw(10)
                 << duration << "\n";
        }

        const vector<string> msgDecrypted = decrypt(msg, sequence);
        saveFile("decrypted.txt", msgDecrypted);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
