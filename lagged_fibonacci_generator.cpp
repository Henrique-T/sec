/*
FIX. It's not working.
TODO:
1. Test with bigger parameters
2. Change variables names from internet code.
3. Comment code (the most possible. Look for good practices)
4. Write note saying I did not have the biggest time to make this project.
*/

#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <bitset>
#include <iostream>
#include <bitset>
#include <tuple>
#include <cassert>

const int SIZE_BITS = 1024;
int LFG(std::string &seed, int &j, int &k, int &m, int &counter);
void printBitset(std::bitset<SIZE_BITS> &bitset);

int main() {

    std::string decimalstring_seed = "29302342342342342342342845784908";
    int j = 3;
    int k = 7;
    int m = 10;
    int counter = 9;

    LFG(decimalstring_seed, j, k, m, counter);

    return 0;
}

int LFG(std::string &seed, int &j, int &k, int &m, int &counter) {
    // Sₙ≡Sₙ₋ⱼ★Sₙ₋ₖ (mod m), 0<j<k
    // The star is either a sum, sub, mul or a XOR.
    int Sn;

    while (counter > 0) {
        for (int i=0; i < seed.size(); i++) {
            if (i == 0) {
                Sn = (seed.at(j-1) * seed.at(k-1)) % m;
            } else if (i > 0 && i < seed.size()-1) {
                seed.at(i) = seed.at(i+1);
            } else {
                seed.at(i) = Sn;
                std::bitset<SIZE_BITS> sn_bitset(Sn);
                printBitset(sn_bitset);
            }
        }
        counter--;
        //LFG(seed, j, k, m, counter);
    }

    return 0;
}

// Print a bitset
void printBitset(std::bitset<SIZE_BITS> &bitset) {
    std::cout << bitset << "\n";
}
