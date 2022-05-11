#ifndef _THREAD_H_
#define _THREAD_H_

#include <functional>
#include <vector>
#include <stack>

#include "coroutine.h"

namespace ccvm {
    enum class thread_status {
        ready, run, done
    };

    class thread {
    public:
        int id;

        thread(std::function<coroutine()> func);
        ~thread();
        thread_status excute();
        thread_status get_status();
    private:
        thread_status status;
        coroutine coro; 
    };
}

#endif // !_THREAD_H_
