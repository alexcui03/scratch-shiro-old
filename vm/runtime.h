#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "thread.h"
#include "variant.h"

namespace ccvm {
    class target;
    
    class runtime {
    public:
        runtime();
        ~runtime();
        void add_target(target *target);
        int push_thread(thread *thread);
        thread *get_thread(int i);
        void free_thread(int i);
        void push_broadcast(std::wstring name, std::function<coroutine()> func);
        void broadcast(std::wstring name);
        std::vector<int> broadcast_and_wait(std::wstring name);
        coroutine ask_and_wait(const std::string &str);
        void excute();
        void terminate();
        bool should_terminate();

        target *stage;
        variant answer;
    private:
        bool terminate_status;

        std::vector<thread *> thread_pool;
        std::stack<int> thread_free;

        std::vector<target *> targets;
        std::map<std::wstring, std::vector<std::function<coroutine()>>> broadcast_map;
    };
}

#endif // !_RUNTIME_H_
