#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "thread.h"

namespace ccvm {
    class target;
    
    class runtime {
    public:
        runtime();
        ~runtime();
        void add_target(target *target);
        void push_thread(thread *thread);
        void push_broadcast(std::wstring name, std::function<coroutine()> func);
        void broadcast(std::wstring name);
        std::vector<thread *> broadcast_and_wait(std::wstring name);
        void excute();
        bool should_terminate();
    private:
        int clone_count;
        std::vector<target *> targets;
        std::vector<thread *> threads;
        std::map<std::wstring, std::vector<std::function<coroutine()>>> broadcast_map;
    };
}

#endif // !_RUNTIME_H_
