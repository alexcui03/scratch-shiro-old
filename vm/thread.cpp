#include "thread.h"

#include <exception>

shiro::thread::thread(std::function<coroutine()> func): coro(func()) {
    this->status = shiro::thread_status::ready;
}

shiro::thread::~thread() {
    this->coro.destroy();
}

shiro::thread_status shiro::thread::excute() {
    if (this->coro.done()) {
        return this->status = shiro::thread_status::done;
    }
    this->coro.promise->throw_if_exception();
    this->status = shiro::thread_status::run;
    this->coro.resume();
    if (this->coro.done()) {
        this->status = shiro::thread_status::done;
    }
    return this->status;
}

shiro::thread_status shiro::thread::get_status() {
    return this->status;
}
