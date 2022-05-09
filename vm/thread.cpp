#include "thread.h"

#include <exception>

ccvm::thread::thread(std::function<coroutine()> func): coro(func()) {
    this->status = ccvm::thread_status::ready;
}

ccvm::thread::~thread() {
    this->coro.destroy();
}

ccvm::thread_status ccvm::thread::excute() {
    if (this->coro.done()) {
        return this->status = ccvm::thread_status::done;
    }
    this->coro.promise->throw_if_exception();
    this->status = ccvm::thread_status::run;
    this->coro.resume();
    if (this->coro.done()) {
        this->status = ccvm::thread_status::done;
    }
    return this->status;
}

ccvm::thread_status ccvm::thread::get_status() {
    return this->status;
}
