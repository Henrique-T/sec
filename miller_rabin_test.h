// Copyright [2021] <Henrique Tridapalli>
#ifndef MR_H
#define MR_H
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <bitset>
#include <iostream>
#include <bitset>
#include <tuple>
#include <cassert>
#include <math.h>
#include "linear_congruential_generator.h"

/*
https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test

## Pseudo code##

write n as 2r·d + 1 with d odd (by factoring out powers of 2 from n − 1)
WitnessLoop: repeat k times:
    pick a random integer a in the range [2, n − 2]
    x ← ad mod n
    if x = 1 or x = n − 1 then
        continue WitnessLoop
    repeat r − 1 times:
        x ← x2 mod n
        if x = n − 1 then
            continue WitnessLoop
    return “composite”
return “probably prime”

## Infos##

This class and namespace use utility functions from Linear Congruential Generator.

*/

namespace prime_test_miller_rabin {

class MR {
    public:

        /* Utility method for x*y % p */
        std::bitset<BIT_SIZE> power(std::bitset<BIT_SIZE> x, std::bitset<BIT_SIZE> y, std::bitset<BIT_SIZE> p, random::LCG &Obj) {
            std::bitset<BIT_SIZE> res;
            for (int i=0; i < y.size(); i++) {
                res.flip(i);
            }

            Obj.gf2_div(x, p, x);
            while (Obj.greaterThanZero(y))
            {
                if (Obj.andBitByBit(y))
                    Obj.gf2_div(Obj.bitsetMultiply(res, x), p, res);
  
                y = y>>1;
                Obj.gf2_div(Obj.bitsetMultiply(x, x), p, x);
            }
            return res;
        }

        /* Applies rules for test */
        bool miller_rabin_test(std::bitset<BIT_SIZE> d, std::bitset<BIT_SIZE> n, random::LCG &Obj) {
            std::bitset<BIT_SIZE> a, one(1), two(2), four(4), n_minus_1, n_minus_4, x, _rand;
            
            // Choose random number in [2..n-2]
            _rand = std::rand();
            n_minus_4 = Obj.bitsetSubtract(n, four);
            n_minus_1 = Obj.bitsetSubtract(n, one);
            a = Obj.bitsetAdd(two, _rand);
            Obj.gf2_div(a, n_minus_4, a);

            // calculate a^d % n
            x = prime_test_miller_rabin::MR::power(a, d, n, Obj);

            // Handle edge cases
            if (Obj.areEquals(x, one) || Obj.areEquals(x, n_minus_1)) 
                return true;
            
            // Loop r-1 times
            while (!Obj.areEquals(d, n_minus_1)) {
                x = Obj.bitsetMultiply(x, x);
                Obj.gf2_div(x, n, x);
                d = Obj.bitsetMultiply(d, two);
                if (Obj.areEquals(x, one)) return false;
                if (Obj.areEquals(x, n_minus_1)) return true;
            }
            return false;
        }

        /* Returns true if number is (probably) prime, false otherwise. */
        bool isPrime(std::bitset<BIT_SIZE> n, int k, random::LCG &Obj) {
            std::bitset<BIT_SIZE> zero, one(1), four(4), two(2), aux_modulo, d;

            // Handle edge cases
            if (Obj.areEquals(n, one))  return true;
            if (Obj.smalleOrEqual(n, 3)) return false;

            // Find r where n = 2^d * r + 1, r >= 1
            d = Obj.bitsetSubtract(n, one);
            while (Obj.areEquals(Obj.gf2_div(d, two, aux_modulo), zero)) {
                d = d>>2;
            }

            // Iterating k times to grow level of accuracy
            for (int i=0;i<k;i++) {
                if (!miller_rabin_test(d,n, Obj)) {
                    return false;
                }
            }
            return true;
        }
};

} // namespace random

#endif


/*

## Example of main function ##


using namespace prime_test_miller_rabin;

int main()
{
    MR obj_MR;
    random::LCG obj_LCG;
    int k = 4;  // Number of iterations
    std::string bitString_a, bitString_Xn, bitString_c, bitString_m;
    int counter;

    bitString_a = "0000000011010101101001010101010101010101010101000010000110101011010010101010101010101010101010";
    bitString_Xn = "0001000011010101101001010101010101010101010101001010000110101011010010101010101010101010101010";
    bitString_c = "0101000011010101101001010101010101010101010101011110000110101011010010101010101010101010101010";
    bitString_m = "1111000011010101101001010101010101010101010101000000000110101011010010101010101010101010101010";

    counter = 1;

    std::bitset<BIT_SIZE> a(bitString_a);
    std::bitset<BIT_SIZE> Xn(bitString_Xn);
    std::bitset<BIT_SIZE> c(bitString_c);
    std::bitset<BIT_SIZE> m(bitString_m);


    std::bitset<BIT_SIZE> rand = obj_LCG.calculate(a, Xn, c, m, counter);

    bool is = obj_MR.isPrime(rand, k, obj_LCG);
    printf("\n%d\n", is);
 
    return 0;
}


*/

