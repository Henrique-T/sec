/*
Apparently everything is working.
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
const int SIZE_BITS = 4096;

void LCG(std::bitset<SIZE_BITS> &a, std::bitset<SIZE_BITS> &Xn, std::bitset<SIZE_BITS> &c, std::bitset<SIZE_BITS> &m, int &counter);
void printBitset(std::bitset<SIZE_BITS> &bitset);
auto fullAdder(bool b1,bool b2,bool &c_in);
int top_bit_set(const std::bitset<SIZE_BITS> &a);
std::bitset<SIZE_BITS> bitsetAdd(std::bitset<SIZE_BITS> &x, const std::bitset<SIZE_BITS> &y);
std::bitset<SIZE_BITS> bitsetMultiply(std::bitset<SIZE_BITS> &x, const std::bitset<SIZE_BITS> &y);
std::bitset<SIZE_BITS> gf2_div(std::bitset<SIZE_BITS> dividend, std::bitset<SIZE_BITS> divisor, std::bitset<SIZE_BITS> &remainder);

int main()
{
    std::string bitString_a, bitString_Xn, bitString_c, bitString_m;
    int counter;

    bitString_a = "00000000110101011010010101010101010101010101010";      // 458800933546
    bitString_Xn = "00010000110101011010010101010101010101010101010";     // 9254893955754
    bitString_c = "01010000110101011010010101010101010101010101010";      // 44439266044586
    bitString_m = "11110000110101011010010101010101010101010101010";      // 132400196266666

    // bitString_a =  "00000000000000000000000000000000000000000000011";   // 3
    // bitString_Xn = "00000000000000000000000000000000000000000000100";   // 4
    // bitString_c =  "00000000000000000000000000000000000000000000110";   // 6
    // bitString_m =  "00000000000000000000000000000000000000000001010";   // 10
    counter = 10;

    std::bitset<SIZE_BITS> a(bitString_a);
    std::bitset<SIZE_BITS> Xn(bitString_Xn);
    std::bitset<SIZE_BITS> c(bitString_c);
    std::bitset<SIZE_BITS> m(bitString_m);

    LCG(a, Xn, c, m, counter);

    return 0;
    
}

// Actual algo.
void LCG(std::bitset<SIZE_BITS> &a, std::bitset<SIZE_BITS> &Xn, std::bitset<SIZE_BITS> &c, std::bitset<SIZE_BITS> &m, int &counter) 
{
    std::bitset<SIZE_BITS> Xn_p_1;
    while (counter > 0) {
        std::bitset<SIZE_BITS> mul = (bitsetMultiply(a, Xn));
        std::bitset<SIZE_BITS> sum = bitsetAdd(mul,c);
        gf2_div(sum, m, Xn_p_1);
        printBitset(Xn_p_1);
        counter--;
        LCG(a, Xn_p_1, c, m, counter);
    }
}

// Print a bitset
void printBitset(std::bitset<SIZE_BITS> &bitset) {
    std::cout << bitset << "\n";
}

// Full adder
auto fullAdder(bool b1,bool b2,bool &c_in) {
    struct result {bool _sum; bool _c_out;};
    bool sum , c_out;
    sum = c_in ^ (b1 ^ b2);
    c_out = (b1 & b2) || (b2 & c_in) || (b1 & c_in);
    return result {sum, c_out};
}

// Add two bitsets
std::bitset<SIZE_BITS> bitsetAdd(std::bitset<SIZE_BITS> &x, const std::bitset<SIZE_BITS> &y) {
    std::bitset<SIZE_BITS> result = x;
    result.reset();
    bool c_in = 0;
    for (int i=0; i < SIZE_BITS; i++) {
        auto sum_result = fullAdder(x[i], y[i], c_in);
        c_in = sum_result._c_out;
        result[i] = sum_result._sum;
    }
    return result;
}

// Multiply two bitsets
std::bitset<SIZE_BITS> bitsetMultiply(std::bitset<SIZE_BITS> &x, const std::bitset<SIZE_BITS> &y) {
    std::bitset<SIZE_BITS> result;
    std::bitset<SIZE_BITS> tmp_sum;
    std::bitset<SIZE_BITS> aux = x;
    result.reset();
    for (int i=0; i < SIZE_BITS; i++)
        if (y[i]) 
            tmp_sum = bitsetAdd(tmp_sum, x << i);
            result = bitsetAdd(result, tmp_sum);
    return result;
}

// Find size of bitset by finding the last active bit.
int top_bit_set(const std::bitset<SIZE_BITS> &a) {
    int i;
    for (i = SIZE_BITS-1; i >= 0; i--)
        if (a.test(i)) break;
    return i;
}

// Divide two numbers and return the remainder(modulo).
std::bitset<SIZE_BITS> gf2_div(std::bitset<SIZE_BITS> dividend, std::bitset<SIZE_BITS> divisor, std::bitset<SIZE_BITS> &remainder) {
    std::bitset<SIZE_BITS> quotient(0);
    int divisor_size = top_bit_set(divisor);

    if (divisor_size < 0) throw;
    int bit;
    while ((bit = top_bit_set(dividend)) >= divisor_size) {
        quotient.set(bit - divisor_size);
        dividend ^= divisor << (bit - divisor_size); }
    remainder = dividend;
    return remainder;
}

/*
DECIMAL VERSION
void LCG(int &a, int &Xn, int &c, int &m, int &counter);

int main() 
{
    int a, Xn, c, m, counter;
    a = 3;
    Xn = 4;
    c = 6;
    m = 10;
    counter = 10;
    LCG(a, Xn, c, m, counter);
    return 0;
}

void LCG(int &a, int &Xn, int &c, int &m, int &counter) 
{
    int Xn_p_1;
    while (counter > 0) {
        Xn_p_1 = (a * Xn + c) % m;
        printf("%d\t", Xn_p_1);
        counter--;
        LCG(a, Xn_p_1, c, m, counter);
    }
}
*/