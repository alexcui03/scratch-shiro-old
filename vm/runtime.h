#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "thread.h"
#include "variant.h"

namespace shiro {
    class target;
    class audio_engine;
    
    class runtime {
    public:
        runtime();
        ~runtime();
        void add_target(target *target, const std::string &name);
        target *get_target(const std::string &name);
        int push_thread(thread *thread);
        thread *get_thread(int i);
        void free_thread(int i);
        void push_broadcast(std::string name, std::function<coroutine()> func);
        void broadcast(std::string name);
        std::vector<int> broadcast_and_wait(std::string name);
        coroutine ask_and_wait(const std::string &str);
        void stop_all_sounds();
        void excute();
        void terminate();
        bool should_terminate();
        void request_redraw();

        target *stage;
        audio_engine *audio;
        variant answer;
        
        bool terminate_status;
        bool need_redraw;

        std::vector<thread *> thread_pool;
        std::stack<int> thread_free;

        std::map<std::string, target *> target_map;
        std::map<std::string, std::vector<std::function<coroutine()>>> broadcast_map;

        target *mouse_target;
        target *random_target;

        void *renderer = nullptr;
    };
}

#endif // !_RUNTIME_H_
