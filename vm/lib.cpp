#include "lib.h"

#include <numbers>
#include <random>

namespace clipcc {

static std::random_device rd;
static std::mt19937 gen(rd());

int random(int a, int b) {
    return std::uniform_int_distribution<int>(a,b)(gen);
}

double random(double a, double b) {
    if (is_int(a) && is_int(b))
        return std::uniform_int_distribution<int>(a,b)(gen);
    else
        return std::uniform_real_distribution<double>(a,b)(gen);
}

variant random(variant a, variant b) {
    if (a.is_int() && b.is_int())
        return std::uniform_int_distribution<int>(a,b)(gen);
    else
        return std::uniform_real_distribution<double>(a,b)(gen);
}

double rad(double x) {
    return x * std::numbers::pi / 180.0;
}

double deg(double x) {
    return x * 180.0 / std::numbers::pi;
}

bool is_int(double x) {
    // in scratch, NaN is always considered as an integer.
    if (std::isnan(x)) return true;
    // integers mod 1 is always 0
    return !std::fmod(x, 1.0);
}

std::string join(const std::string &a, const std::string &b) {
    return a + b;
}

}
