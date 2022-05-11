#include <iostream>
#include <functional>
#include <coroutine>

#include "runtime.h"
#include "target.h"

using namespace ccvm;

class sprite1: public sprite {
public:
    sprite1(runtime *rt): sprite(rt) {}
    int var1 = 0, var2 = 1, var3 = 2, var4 = 3;
};

coroutine when_green_flag(target *target) {
    {
        auto waiting_threads = target->broadcast_and_wait(L"dd");
        while (true) {
            co_await std::suspend_always();
            if (target->check_waiting_threads(waiting_threads)) break;
        }
    }
    for (int i = 0; i < 10; ++i) {
        std::cout << "say " << i << std::endl;
        co_await std::suspend_always();
    }
    co_return;
}

coroutine when_broadcast(target *target) {
    for (int i = 0; i < 3; ++i) {
        std::cout << "broadcast say " << i << std::endl;
        co_await std::suspend_always();
    }
    co_return;
}

coroutine when_broadcast2(target *target) {
    for (int i = 0; i < 7; ++i) {
        std::cout << "broadcast2 say " << i << std::endl;
        co_await std::suspend_always();
    }
    co_return;
}

int main() {
    runtime runtime;
    sprite1 sprite1(&runtime);
    runtime.push_broadcast(L"dd", std::bind(when_broadcast, &sprite1));
    runtime.push_broadcast(L"dd", std::bind(when_broadcast2, &sprite1));

    thread *t1 = new thread(std::bind(when_green_flag, &sprite1));
    //thread *t2 = new thread(std::bind(when_broadcast, &sprite1));
    runtime.push_thread(t1);
    //runtime.push_thread(t2);
    std::cout << "start" << std::endl;
    while (!runtime.should_terminate()) {
        std::cout << "step" << std::endl;
        runtime.excute();
    }
    std::cout << "end" << std::endl;
    return 0;
}
