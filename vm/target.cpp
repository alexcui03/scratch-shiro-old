#include "target.h"

#include "runtime.h"
#include "thread.h"

ccvm::target::target(ccvm::runtime *rt) {
    this->rt = rt;
}

ccvm::target::~target() {

}

void ccvm::target::broadcast(std::wstring name) {
    this->rt->broadcast(name);
}

std::vector<ccvm::thread *> ccvm::target::broadcast_and_wait(std::wstring name) {
    return this->rt->broadcast_and_wait(name);
}

ccvm::stage::stage(ccvm::runtime *rt): target(rt) {
    
}

ccvm::stage::~stage() {

}

ccvm::sprite::sprite(ccvm::runtime *rt): target(rt) {
    
}

ccvm::sprite::~sprite() {

}
