#include <cassert>

#include "../../compiler/util.h"

int main() {
    assert(clipcc::is_valid_number("1"));
    assert(clipcc::is_valid_number("123"));
    assert(clipcc::is_valid_number("-1"));
    assert(clipcc::is_valid_number("-123"));
    assert(clipcc::is_valid_number("0"));
    assert(clipcc::is_valid_number("25.03"));
    assert(clipcc::is_valid_number("-25.03"));

    assert(!clipcc::is_valid_number("b-25.03"));
    
    return 0;
}

