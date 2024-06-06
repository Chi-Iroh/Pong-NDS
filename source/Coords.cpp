#include "Coords.hpp"

// max is included
unsigned add(unsigned n1, unsigned n2, unsigned max) {
    n1 %= (max + 1);
    n2 %= (max + 1);
    return (n1 + n2) % (max + 1);
}

// max is included
unsigned sub(unsigned n1, unsigned n2, unsigned max) {
    if (n2 > n1) {
        return (max + 1) - ((n2 - n1) % (max + 1));
    }
    return (n1 - n2) % (max + 1);
}

unsigned diff(unsigned n1, unsigned n2) {
    return (n1 > n2) ? n1 - n2 : n2 - n1;
}