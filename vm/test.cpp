#include <iostream>
#include <functional>
#include <coroutine>

#include "clipcc_vm.h"

using namespace ccvm;

class target_0: public ccvm::stage {
public:
target_0(ccvm::runtime *rt): ccvm::stage(rt) {
}
~target_0() {
}
};
class target_1: public ccvm::sprite {
public:
target_0 *stage;
target_1(ccvm::runtime *rt, target_0 *t): ccvm::sprite(rt), stage(t) {
}
~target_1() {
}
ccvm::coroutine func_0(const ccvm::variant &arg_0, const ccvm::variant &arg_1) {
co_return;
}
ccvm::coroutine func_1() {
say("Hello!");
co_return;
}
ccvm::coroutine func_2(const ccvm::variant &arg_0) {
say(arg_0);
co_return;
}
ccvm::coroutine func_3() {
co_yield func_1();
co_yield func_2("p");
co_return;
}
};
int main() {
ccvm::runtime runtime;
target_0 target_0(&runtime);
target_1 target_1(&runtime, &target_0);
runtime.push_thread(new ccvm::thread(std::bind(&target_1::func_3, &target_1)));
while (!runtime.should_terminate()) runtime.excute();
return 0;
}
