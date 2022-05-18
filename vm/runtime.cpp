#include "runtime.h"

#include <future>
#include <string>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

ccvm::runtime::runtime() {
    this->terminate_status = false;
}

ccvm::runtime::~runtime() {

}

void ccvm::runtime::add_target(target *target) {
    this->targets.push_back(target);
}

int ccvm::runtime::push_thread(thread *thread) {
    /*if (this->thread_free.size()) {
        const int i = this->thread_free.top();
        this->thread_free.pop();
        if (this->thread_pool[i] != nullptr) delete this->thread_pool[i];
        this->thread_pool[i] = thread;
        thread->id = i;
    } else*/ {
        this->thread_pool.push_back(thread);
        thread->id = this->thread_pool.size() - 1;
    }
    return thread->id;
}

ccvm::thread *ccvm::runtime::get_thread(int i) {
    return this->thread_pool[i];
}

void ccvm::runtime::free_thread(int i) {
    delete this->thread_pool[i];
    this->thread_pool[i] = nullptr;
    this->thread_free.push(i);
}

void ccvm::runtime::push_broadcast(std::wstring name, std::function<coroutine()> func) {
    this->broadcast_map[name].push_back(func);
}

void ccvm::runtime::broadcast(std::wstring name) {
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            this->push_thread(new thread(func));
        }
    }
}

std::vector<int> ccvm::runtime::broadcast_and_wait(std::wstring name) {
    std::vector<int> list;
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            list.push_back(this->push_thread(new thread(func)));
        }
    }
    return list;
}

ccvm::coroutine ccvm::runtime::ask_and_wait(const std::string &str) {
    auto io = std::async([&]() {
        std::string s;
        std::getline(std::cin, s);
        return s;
    });
    std::cout << str << std::endl;
    while (true) {
        if (io.wait_for(0s) == std::future_status::ready) {
            this->answer = io.get();
            co_return;
        }
        co_await std::suspend_always{};
    }
}

void ccvm::runtime::excute() {
    std::vector<int> done_thread;
    for (int i = 0; i < this->thread_pool.size(); ++i) {
        auto thread = this->thread_pool[i];
        if (thread == nullptr) continue;
        thread->excute();
        if (thread->get_status() == thread_status::done) {
            done_thread.push_back(thread->id);
        }
    }
    for (auto id : done_thread) {
        this->free_thread(id);
    }
}

void ccvm::runtime::terminate() {
    this->terminate_status = true;
}

bool ccvm::runtime::should_terminate() {
    return this->terminate_status;
}
