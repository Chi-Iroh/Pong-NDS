#include "Coords.hpp"

int modulus(int n, int mod) {
    while (n < 0) {
        n += mod;
    }
    return n % mod;
}

int diff(int n1, int n2) {
    return (n1 > n2) ? n1 - n2 : n2 - n1;
}