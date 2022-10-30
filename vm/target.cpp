#include "target.h"

#include <iostream>
#include <string>
#include <map>

#include "runtime.h"
#include "thread.h"

#include "../player/target_impl.h"
#include "../player/audio_engine.h"

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

void target::load_costume(const std::string &name, const std::string &path, int resolution) {
    impl->load_costume(path, resolution);
    this->costume_list.push_back(name);
}

void target::load_sound(const std::string &name, const std::string &path) {
    int sound_id = this->runtime->audio->load(path);
    this->sound_list.push_back(name);
    this->sound_id_map[name] = sound_id;
}

void target::play_sound(const std::string &name) {
    if (this->current_sound_id > 0) {
        if (this->runtime->audio->is_playing(this->current_sound_id)) {
            this->runtime->audio->stop(this->current_sound_id);
        }
    }
    if (this->sound_id_map.find(name) != this->sound_id_map.end()) {
        this->current_sound_id = this->sound_id_map[name];
    }
    else {
        this->current_sound_id = this->sound_id_map[this->sound_list[std::atoi(name.c_str())]];
    }
    this->runtime->audio->play(this->current_sound_id);
}

void target::stop_all_sounds() {
    this->runtime->stop_all_sounds();
}

void target::set_costume(int x) {
    this->current_costume = x;
    impl->set_costume(x);
}

stage::stage(shiro::runtime *rt): target(rt) {
    this->runtime->stage = this;
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
