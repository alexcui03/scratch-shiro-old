#include "target.h"

#include <iostream>
#include "runtime.h"
#include "thread.h"

#include "../player/target_impl.h"

namespace shiro {

target::target(shiro::runtime *rt) {
    this->runtime = rt;
    this->impl = new target_impl(this);
}

target::~target() {

}

void target::request_redraw() {
    this->need_redraw = true;
    if (this->visible) {
        this->runtime->request_redraw();
    }
}

void target::broadcast(const std::string &name) {
    this->runtime->broadcast(name);
}

std::vector<int> target::broadcast_and_wait(const std::string &name) {
    return this->runtime->broadcast_and_wait(name);
}

bool target::check_waiting_threads(const std::vector<int> &waiting) {
    for (auto i : waiting) {
        if (this->runtime->get_thread(i) != nullptr) return false;
    }
    return true;
}

void target::load_costume(const std::string &name, const std::string &path) {
    impl->load_costume(path);
    this->costume_map.push_back(name);
}

void target::load_sound(const std::string &name, const std::string &path) {
    this->sound_map.push_back(name);
}

void target::set_costume(int x) {
    this->current_costume = x - 1;
    impl->set_costume(x - 1);
}

stage::stage(shiro::runtime *rt): target(rt) {
    runtime->stage = this;
}

stage::~stage() {

}

sprite::sprite(shiro::runtime *rt): target(rt) {

}

sprite::~sprite() {

}

void sprite::say(const std::string &str) {
    std::cout << str << std::endl;
}

}
