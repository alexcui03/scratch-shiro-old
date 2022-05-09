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
        void broadcast(std::wstring name);
        std::vector<thread *> broadcast_and_wait(std::wstring name);
    protected:
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
