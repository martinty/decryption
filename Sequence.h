#pragma once
#include <algorithm>  // std::swap
#include <vector>     // std::vector

// https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

class Sequence {
   private:
    std::vector<std::vector<int>> seqs;
    std::vector<int> comb;
    int size = 0;
    void permute(int l, int r) {
        if (l == r)
            seqs.push_back(comb);
        else {
            for (int i = l; i <= r; i++) {
                std::swap(comb[l], comb[i]);
                permute(l + 1, r);
                std::swap(comb[l], comb[i]);
            }
        }
    }

   public:
    Sequence() = default;
    ~Sequence() = default;
    Sequence(int n) { generate(n); }
    void generate(int n) {
        seqs.clear();
        comb.clear();
        for (int i = 0; i < n; i++) {
            comb.push_back(i);
        }
        permute(0, comb.size() - 1);
        size = seqs.size();
    }
    std::vector<std::vector<int>> get() const { return seqs; }
};
