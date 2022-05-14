#ifndef _TARGET_H_
#define _TARGET_H_

#include <string>
#include <functional>

#include "runtime.h"

namespace ccvm {
    class target {
    public:
        target(runtime *rt);
        ~target();
        void broadcast(const std::wstring &name);
        std::vector<int> broadcast_and_wait(const std::wstring &name);
        bool check_waiting_threads(const std::vector<int> &waiting);
    public:
        runtime *rt;
    };

    class stage: public target {
    public:
        stage(runtime *rt);
        ~stage();
    };

    class sprite: public target {
    public:
        sprite(runtime *rt);
        ~sprite();
    public:
        double x;
        double y;
        double direction;
    };
}

#endif // !_TARGET_H_
