// Copyright [2021] <Henrique Tridapalli>
#ifndef LCG_H
#define LCG_H
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <bitset>
#include <iostream>
#include <bitset>
#include <tuple>
#include <cassert>
#include <vector>
#include <cstdio>
#include <ctime>


/*

https://en.wikipedia.org/wiki/Linear_congruential_generator

Python code:

from typing import Generator

def lcg(modulus: int, a: int, c: int, seed: int) -> Generator[int, None, None]:
    """Linear congruential generator."""
    while True:
        seed = (a * seed + c) % modulus
        yield seed

*/

const int BIT_SIZE = 4096;

namespace random {

class LCG {
    public:

        /* Applies rules for generator */
        std::bitset<BIT_SIZE> calculate(std::bitset<BIT_SIZE> &a, std::bitset<BIT_SIZE> &Xn, std::bitset<BIT_SIZE> &c, std::bitset<BIT_SIZE> &m, int &counter) {
            std::bitset<BIT_SIZE> Xn_p_1;
            std::clock_t start;
            double duration;
            start = std::clock();

            while (counter > 0) {
                std::bitset<BIT_SIZE> mul = (random::LCG::bitsetMultiply(a, Xn));
                std::bitset<BIT_SIZE> sum = random::LCG::bitsetAdd(mul,c);
                // calculate modulo
                random::LCG::gf2_div(sum, m, Xn_p_1);
                random::LCG::printBitset(Xn_p_1);
                counter--;
                random::LCG::calculate(a, Xn_p_1, c, m, counter);
            }
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            std::cout<<"duration= "<< duration <<'\n';
            return Xn_p_1;
        }

        /* Utility method for printing bitset class */
        void printBitset(std::bitset<BIT_SIZE> &bitset) {
            std::cout << bitset << "\n";
        }

        /* Utility Full adder to be applied on each two bits of two bitsets */
        auto fullAdder(bool b1,bool b2,bool &c_in) {
            struct result {bool _sum; bool _c_out;};
            bool sum , c_out;
            sum = c_in ^ (b1 ^ b2);
            c_out = (b1 & b2) || (b2 & c_in) || (b1 & c_in);
            return result {sum, c_out};
        }

        /* Utility method for adding two bitsets */
        std::bitset<BIT_SIZE> bitsetAdd(std::bitset<BIT_SIZE> &x, const std::bitset<BIT_SIZE> &y) {
            std::bitset<BIT_SIZE> result = x;
            result.reset();
            bool c_in = 0;
            for (int i=0; i < BIT_SIZE; i++) {
                auto sum_result = random::LCG::fullAdder(x[i], y[i], c_in);
                c_in = sum_result._c_out;
                result[i] = sum_result._sum;
            }
            return result;
        }

        /* Utility method for subtracting two bitsets */
        std::bitset<BIT_SIZE> bitsetSubtract(std::bitset<BIT_SIZE> &x, std::bitset<BIT_SIZE> &y) {
            // invert bits
            for (int i=0; i < y.size(); i++) {
                y.flip(i);
            }
            std::bitset<BIT_SIZE> one(1);
            //sum one
            y = random::LCG::bitsetAdd(y, one);
            // sum x and y
            return random::LCG::bitsetAdd(x, y);
        }

        /* Utility method for multiplying two bitsets */
        std::bitset<BIT_SIZE> bitsetMultiply(std::bitset<BIT_SIZE> &x, const std::bitset<BIT_SIZE> &y) {
            std::bitset<BIT_SIZE> result;
            std::bitset<BIT_SIZE> tmp_sum;
            std::bitset<BIT_SIZE> aux = x;
            result.reset();
            for (int i=0; i < BIT_SIZE; i++)
                if (y[i]) 
                    tmp_sum = random::LCG::bitsetAdd(tmp_sum, x << i);
                    result = random::LCG::bitsetAdd(result, tmp_sum);
            return result;
        }

        /* Utility method for testing equality */
        bool areEquals(const std::bitset<BIT_SIZE> &x, std::bitset<BIT_SIZE> &y)
        {
            for (int i = BIT_SIZE-1; i >= 0; i--) {
                if (x[i] ^ y[i]) return y[i];
            }
            return false;
        }

        /* Utility method for findind the most significant bit 1 */
        int top_bit_set(const std::bitset<BIT_SIZE> &a) {
            int i;
            for (i = BIT_SIZE-1; i >= 0; i--)
                if (a.test(i)) break;
            return i;
        }

        /* Utility method for checking > 0 */
        bool greaterThanZero(const std::bitset<BIT_SIZE> &a) {
            for (int i=0; i < a.size()-1; i++) {
                if (a.test(i)) return true;
            }
            return false;
        }

        /* Utility method for logical and */
        bool andBitByBit(std::bitset<BIT_SIZE> &a) {
            for (int i=0; i < BIT_SIZE; i++) {
                return a[i] & 1;
            }
            return false;
        }

        /* Utility method for <= */
        bool smalleOrEqual(std::bitset<BIT_SIZE> &a, int number) {
            std::bitset<BIT_SIZE> one(number);
            for (int i=0; i < BIT_SIZE-2; i++) {
                if (a[i] == '1') {
                    return true;
                }
            }
            return false;
        }

        /* Utility method for modulo */
        std::bitset<BIT_SIZE> gf2_div(std::bitset<BIT_SIZE> dividend, std::bitset<BIT_SIZE> divisor, std::bitset<BIT_SIZE> &remainder) {
            std::bitset<BIT_SIZE> quotient(0);
            int divisor_size = random::LCG::top_bit_set(divisor);

            if (divisor_size < 0) throw;
            int bit;
            while ((bit = random::LCG::top_bit_set(dividend)) >= divisor_size) {
                quotient.set(bit - divisor_size);
                dividend ^= divisor << (bit - divisor_size); }
            remainder = dividend;
            return remainder;
        }
};

} // namespace random

#endif

/*

## Example of main function ##

using namespace random;

int main() {
    // LCG obj;
    // std::string bitString_a, bitString_Xn, bitString_c, bitString_m;
    // int counter;

    // bitString_a = "00000000110101011010010101010101010101010101010";      // 458800933546
    // bitString_Xn = "00010000110101011010010101010101010101010101010";     // 9254893955754
    // bitString_c = "01010000110101011010010101010101010101010101010";      // 44439266044586
    // bitString_m = "11110000110101011010010101010101010101010101010";      // 132400196266666

    // counter = 1;

    // std::bitset<BIT_SIZE> a(bitString_a);
    // std::bitset<BIT_SIZE> Xn(bitString_Xn);
    // std::bitset<BIT_SIZE> c(bitString_c);
    // std::bitset<BIT_SIZE> m(bitString_m);
    // obj.calculate(a, Xn, c, m, counter);
    return 0;
}

*/
