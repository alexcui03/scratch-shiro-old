#include "thread.h"

#include <exception>

clipcc::thread::thread(std::function<coroutine()> func): coro(func()) {
    this->status = clipcc::thread_status::ready;
}

clipcc::thread::~thread() {
    this->coro.destroy();
}

clipcc::thread_status clipcc::thread::excute() {
    if (this->coro.done()) {
        return this->status = clipcc::thread_status::done;
    }
    this->coro.promise->throw_if_exception();
    this->status = clipcc::thread_status::run;
    this->coro.resume();
    if (this->coro.done()) {
        this->status = clipcc::thread_status::done;
    }
    return this->status;
}

clipcc::thread_status clipcc::thread::get_status() {
    return this->status;
}
