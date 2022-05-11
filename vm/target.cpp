#include "target.h"

#include "runtime.h"
#include "thread.h"

ccvm::target::target(ccvm::runtime *rt) {
    this->rt = rt;
}

ccvm::target::~target() {

}

void ccvm::target::broadcast(const std::wstring &name) {
    this->rt->broadcast(name);
}

std::vector<int> ccvm::target::broadcast_and_wait(const std::wstring &name) {
    return this->rt->broadcast_and_wait(name);
}

bool ccvm::target::check_waiting_threads(const std::vector<int> &waiting) {
    for (auto i : waiting) {
        if (this->rt->get_thread(i) != nullptr) return false;
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
