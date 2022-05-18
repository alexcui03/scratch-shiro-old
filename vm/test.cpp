#include <iostream>
#include <functional>
#include <coroutine>

#include "clipcc_vm.h"

using namespace ccvm;

class target_0 : public ccvm::stage
{
public:
    ccvm::variant var_0;
    ccvm::variant var_1;
    target_0(ccvm::runtime *rt) : ccvm::stage(rt)
    {
    }
    ~target_0()
    {
    }
};
class target_1 : public ccvm::sprite
{
public:
    target_0 *stage;
    target_1(ccvm::runtime *rt, target_0 *t) : ccvm::sprite(rt), stage(t)
    {
    }
    ~target_1()
    {
    }
    ccvm::coroutine func_0()
    {
        this->stage->var_0 = ccvm::random((1), (10));
        co_return;
    }
    ccvm::coroutine func_1()
    {
        co_return;
    }
    ccvm::coroutine func_2()
    {
        co_return;
    }
};
class target_2 : public ccvm::sprite
{
public:
    target_0 *stage;
    target_2(ccvm::runtime *rt, target_0 *t) : ccvm::sprite(rt), stage(t)
    {
    }
    ~target_2()
    {
    }
    ccvm::coroutine func_0()
    {
        co_yield runtime->ask_and_wait("input a");
        this->stage->var_1 = this->runtime->answer;
        co_yield runtime->ask_and_wait("input b");
        this->say(static_cast<double>(this->stage->var_1) + static_cast<double>(this->runtime->answer));
        co_return;
    }
};

ccvm::coroutine func_0(ccvm::runtime *runtime)
{
    co_yield runtime->ask_and_wait("input a");
    co_return;
}
int main()
{
    ccvm::runtime runtime;
    target_0 target_0(&runtime);
    target_1 target_1(&runtime, &target_0);
    runtime.push_thread(new ccvm::thread(std::bind(&target_1::func_0, &target_1)));
    target_2 target_2(&runtime, &target_0);
    runtime.push_thread(new ccvm::thread(std::bind(&target_2::func_0, &target_2)));
    while (!runtime.should_terminate())
        runtime.excute();
    return 0;
}