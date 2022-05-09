#include "runtime.h"

ccvm::runtime::runtime() {

}

ccvm::runtime::~runtime() {

}

void ccvm::runtime::add_target(target *target) {
    this->targets.push_back(target);
}

void ccvm::runtime::push_thread(thread *thread) {
    this->threads.push_back(thread);
}

void ccvm::runtime::push_broadcast(std::wstring name, std::function<coroutine()> func) {
    this->broadcast_map[name].push_back(func);
}

void ccvm::runtime::broadcast(std::wstring name) {
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            this->threads.push_back(new thread(func));
        }
    }
}

std::vector<ccvm::thread *> ccvm::runtime::broadcast_and_wait(std::wstring name) {
    std::vector<ccvm::thread *> list;
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            thread *th = new thread(func);
            this->threads.push_back(th);
            list.push_back(th);
        }
    }
    return list;
}

void ccvm::runtime::excute() {
    for (auto thread : this->threads) {
        if (thread == nullptr || thread->get_status() == thread_status::done) {
            continue;
        }
        thread->excute();
    }
}

bool ccvm::runtime::should_terminate() {
    return false;
}
