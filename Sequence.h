#pragma once
#include <algorithm>  // std::swap
#include <vector>     // std::vector

// https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

class Sequence {
   private:
    std::vector<std::vector<int>> seq;
    std::vector<int> comb;
    void permute(int l, int r) {
        if (l == r)
            seq.push_back(comb);
        else {
            for (int i = l; i <= r; i++) {
                std::swap(comb[l], comb[i]);
                permute(l + 1, r);
                std::swap(comb[l], comb[i]);
            }
        }
    }

   public:
    Sequence() = delete;
    ~Sequence() = default;
    Sequence(int n) {
        for (int i{0}; i < n; i++) {
            comb.push_back(i);
        }
        permute(0, comb.size() - 1);
    }
    std::vector<std::vector<int>> get() { return seq; }
};
