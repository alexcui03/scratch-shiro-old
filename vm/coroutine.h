#ifndef _COROUTINE_H_
#define _COROUTINE_H_

#include <exception>
#include <coroutine>

namespace ccvm {
    class promise;

    class coroutine : public std::coroutine_handle<promise> {
    public:
        using promise_type = ::ccvm::promise;
    };

    class promise {
    public:
        std::exception_ptr exception;

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {
            this->exception = std::current_exception();
        }
        coroutine get_return_object() {
            return {coroutine::from_promise(*this)};
        }
        void return_void() {}
    };
}

#endif // !_COROUTINE_H_
