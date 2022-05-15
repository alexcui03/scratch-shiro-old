#include "lib.h"

#include <random>

namespace ccvm {

static std::random_device rd;
static std::mt19937 gen(rd());

int random(int a, int b) {
    return std::uniform_int_distribution<int>(a,b)(gen);
}

double random(double a, double b) {
    return std::uniform_real_distribution<double>(a,b)(gen);
}

variant random(variant a, variant b) {
    if (a.isInt() && b.isInt())
        return random(static_cast<int>(a), static_cast<int>(b));
    else
        return random(static_cast<double>(a), static_cast<double>(b));
}

}
