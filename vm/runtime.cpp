#include "runtime.h"

#include <future>
#include <string>
#include <chrono>
#include <iostream>

#include "target.h"

using namespace std::chrono_literals;

clipcc::runtime::runtime() {
    this->terminate_status = false;
    this->mouse_target = new clipcc::target(this);
    this->random_target = new clipcc::target(this);
}

clipcc::runtime::~runtime() {

}

void clipcc::runtime::add_target(target *target, const std::string &name) {
    this->target_map[name] = target;
}

clipcc::target *clipcc::runtime::get_target(const std::string &name) {
    if (name == "mouse") {
        return this->mouse_target;
    }
    if (name == "random") {
        return this->random_target;
    }
    if (this->target_map.contains(name)) {
        return this->target_map[name];
    }
}

int clipcc::runtime::push_thread(thread *thread) {
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

clipcc::thread *clipcc::runtime::get_thread(int i) {
    return this->thread_pool[i];
}

void clipcc::runtime::free_thread(int i) {
    delete this->thread_pool[i];
    this->thread_pool[i] = nullptr;
    this->thread_free.push(i);
}

void clipcc::runtime::push_broadcast(std::string name, std::function<coroutine()> func) {
    this->broadcast_map[name].push_back(func);
}

void clipcc::runtime::broadcast(std::string name) {
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            this->push_thread(new thread(func));
        }
    }
}

std::vector<int> clipcc::runtime::broadcast_and_wait(std::string name) {
    std::vector<int> list;
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            list.push_back(this->push_thread(new thread(func)));
        }
    }
    return list;
}

clipcc::coroutine clipcc::runtime::ask_and_wait(const std::string &str) {
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

void clipcc::runtime::excute() {
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

void clipcc::runtime::terminate() {
    this->terminate_status = true;
}

bool clipcc::runtime::should_terminate() {
    return this->terminate_status;
}

void clipcc::runtime::request_redraw() {
    this->need_redraw = true;
}
