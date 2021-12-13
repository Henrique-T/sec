// Miller-rabin
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

// extern int SIZE_BITS = 4096;

namespace prime_test_miller_rabin {

class MR {
    public:
        int power(int x, unsigned int y, int p) {
            int res = 1;
            x = x % p;

            while (y > 0)
            {
                if (y & 1)
                    res = (res*x) % p;
  
                y = y>>1;
                x = (x*x) % p;
            }
            return res;
        }
        bool miller_rabin_test(int d, int n) {
            //int a = 2 + rand() % (n - 4);
            int a = 2 + () % (n - 4);
            int x = power(a, d, n);
            if (x == 1 || x == n-1) 
                return true;
            
            while (d != n-1) {
                x = (x * x) % n;
                d *= 2;
                if (x == 1) return false;
                if (x == n-1) return true;
            }
            return false;
        }

        bool isPrime(int n, int k) {
            if (n <= 1 || n == 4)  return false;
            if (n <= 3) return true;

            int d = n - 1;
            while (d % 2 == 0) {
                d /= 2;
            }
            for (int i=0;i<k;i++) {
                if (!miller_rabin_test(d,n)) {
                    return false;
                }
            }
            return true;
        }
};

} // namespace random

#endif

// using namespace prime_test_miller_rabin;
// // Driver program
// int main()
// {
//     MR obj;
//     int k = 4;  // Number of iterations
 
//     std::cout << "All primes smaller than 100: \n";
//     for (int n = 1; n < 100; n++)
//        if (obj.isPrime(n, k))
//           std::cout << n << " ";
 
//     return 0;
// }