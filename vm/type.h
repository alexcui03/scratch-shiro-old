#ifndef _TYPE_H_
#define _TYPE_H_

#include <cstdlib>

#include "coroutine.h"

namespace ccvm {
    using cc_statement = coroutine;
    using size_t = std::size_t;
}

#endif // !_TYPE_H_
