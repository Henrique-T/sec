// Generate with Linear Congruential Generator and test it with Miller-Rabin.

#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <bitset>
#include <iostream>
#include <bitset>
#include <tuple>
#include <cassert>
#include <vector>
#include "miller_rabin_test.h"
#include "linear_congruential_generator.h"

int main()
{
    std::clock_t start;
    double duration;
    start = std::clock();

    prime_test_miller_rabin::MR obj_MR;
    random::LCG obj_LCG;
    int k = 4;  // Number of iterations
    std::string bitString_a, bitString_Xn, bitString_c, bitString_m;
    int counter;

    bitString_a = "00000000110101011010010101010101010101010101010000100001101010110100101010101010101010101010100101000011010101101001010101010101010101010101011110000110101011010010101010101010101010101010";
    bitString_Xn = "00010000110101011010010101010101010101010101010010100001101010110100101010101010101010101010100101000011010101101001010101010101010101010101011110000110101011010010101010101010101010101010";
    bitString_c = "01010000110101011010010101010101010101010101010111100001101010110100101010101010101010101010100101000011010101101001010101010101010101010101011110000110101011010010101010101010101010101010";
    bitString_m = "11110000110101011010010101010101010101010101010000000001101010110100101010101010101010101010100000000011010101101001010101010101010101010101000010000110101011010010101010101010101010101010";

    counter = 1;

    std::bitset<BIT_SIZE> a(bitString_a);
    std::bitset<BIT_SIZE> Xn(bitString_Xn);
    std::bitset<BIT_SIZE> c(bitString_c);
    std::bitset<BIT_SIZE> m(bitString_m);


    std::bitset<BIT_SIZE> rand = obj_LCG.calculate(a, Xn, c, m, counter);

    bool is = obj_MR.isPrime(rand, k, obj_LCG);
    printf("\n%d\n", is);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"\nduration2= "<< duration <<'\n';
 
    return 0;
}