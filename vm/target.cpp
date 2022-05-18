#include "target.h"

#include <iostream>
#include "runtime.h"
#include "thread.h"

ccvm::target::target(ccvm::runtime *rt) {
    this->runtime = rt;
}

ccvm::target::~target() {

}

void ccvm::target::broadcast(const std::wstring &name) {
    this->runtime->broadcast(name);
}

std::vector<int> ccvm::target::broadcast_and_wait(const std::wstring &name) {
    return this->runtime->broadcast_and_wait(name);
}

bool ccvm::target::check_waiting_threads(const std::vector<int> &waiting) {
    for (auto i : waiting) {
        if (this->runtime->get_thread(i) != nullptr) return false;
    }
    return true;
}

ccvm::stage::stage(ccvm::runtime *rt): target(rt) {
    
}

ccvm::stage::~stage() {

}

ccvm::sprite::sprite(ccvm::runtime *rt): target(rt) {
    
}

ccvm::sprite::~sprite() {

}

void ccvm::sprite::say(const std::string &str) {
    std::cout << str << std::endl;
}
