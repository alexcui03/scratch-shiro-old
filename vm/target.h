#ifndef _TARGET_H_
#define _TARGET_H_

#include <string>
#include <functional>
#include <type_traits>

#include "runtime.h"

namespace ccvm {
    template <class Ty>
    concept arithmetic = std::is_arithmetic_v<Ty>;

    class target {
    public:
        target(runtime *rt);
        ~target();
        void broadcast(const std::wstring &name);
        std::vector<int> broadcast_and_wait(const std::wstring &name);
        bool check_waiting_threads(const std::vector<int> &waiting);
    public:
        runtime *runtime;
    };

    class stage: public target {
    public:
        stage(ccvm::runtime *rt);
        ~stage();
    };

    class sprite: public target {
    public:
        sprite(ccvm::runtime *rt);
        ~sprite();

        // blocks
        void say(const std::string &str);
        template <arithmetic Ty> void say(const Ty &val) {
            this->say(std::to_string(val));
        }
    public:
        double x;
        double y;
        double direction;
    };
}

#endif // !_TARGET_H_
