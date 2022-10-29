#include "runtime.h"

#include <future>
#include <string>
#include <chrono>
#include <iostream>

#include "target.h"

using namespace std::chrono_literals;

shiro::runtime::runtime() {
    this->terminate_status = false;
    this->mouse_target = new shiro::target(this);
    this->random_target = new shiro::target(this);
}

shiro::runtime::~runtime() {

}

void shiro::runtime::add_target(target *target, const std::string &name) {
    this->target_map[name] = target;
}

shiro::target *shiro::runtime::get_target(const std::string &name) {
    if (name == "mouse") {
        return this->mouse_target;
    }
    if (name == "random") {
        return this->random_target;
    }
    if (this->target_map.contains(name)) {
        return this->target_map[name];
    }
    return nullptr;
}

int shiro::runtime::push_thread(thread *thread) {
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

shiro::thread *shiro::runtime::get_thread(int i) {
    return this->thread_pool[i];
}

void shiro::runtime::free_thread(int i) {
    delete this->thread_pool[i];
    this->thread_pool[i] = nullptr;
    this->thread_free.push(i);
}

void shiro::runtime::push_broadcast(std::string name, std::function<coroutine()> func) {
    this->broadcast_map[name].push_back(func);
}

void shiro::runtime::broadcast(std::string name) {
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            this->push_thread(new thread(func));
        }
    }
}

std::vector<int> shiro::runtime::broadcast_and_wait(std::string name) {
    std::vector<int> list;
    auto funcs = this->broadcast_map.find(name);
    if (funcs != this->broadcast_map.end()) {
        for (auto &func : funcs->second) {
            list.push_back(this->push_thread(new thread(func)));
        }
    }
    return list;
}

shiro::coroutine shiro::runtime::ask_and_wait(const std::string &str) {
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

void shiro::runtime::excute() {
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

void shiro::runtime::terminate() {
    this->terminate_status = true;
}

bool shiro::runtime::should_terminate() {
    return this->terminate_status;
}

void shiro::runtime::request_redraw() {
    this->need_redraw = true;
}
