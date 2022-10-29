#include <cassert>

#include "../../compiler/util.h"

int main() {
    assert(shiro::is_valid_number("1"));
    assert(shiro::is_valid_number("123"));
    assert(shiro::is_valid_number("-1"));
    assert(shiro::is_valid_number("-123"));
    assert(shiro::is_valid_number("0"));
    assert(shiro::is_valid_number("25.03"));
    assert(shiro::is_valid_number("-25.03"));

    assert(!shiro::is_valid_number("b-25.03"));
    
    return 0;
}

