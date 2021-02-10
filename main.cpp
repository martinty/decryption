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
        const vector<string> dict = loadDict("american-english");

        const auto startTime = chrono::steady_clock::now();

        // Work start
        const vector<int> sequence = findCorrectSequence(msg, dict);
        // Work done

        const auto endTime = chrono::steady_clock::now();
        const auto diffTime =
            chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        const double duration = diffTime.count() / 1000.0;

        if (PRINT) {
            cout << fixed << setprecision(3);
            cout << setw(30) << "Elapsed time in seconds: " << setw(10)
                 << duration << "\n";
        }

        const vector<string> msgDecrypted = decrypt(msg, sequence);
        saveFile("msg-decrypted.txt", msgDecrypted);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
    } catch (...) {
        cerr << "Unknown error!\n";
    }

    return 0;
}
