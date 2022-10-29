#ifndef _TARGET_H_
#define _TARGET_H_

#include <string>
#include <functional>
#include <type_traits>

#include "runtime.h"

namespace shiro {
    template <class Ty>
    concept arithmetic = std::is_arithmetic_v<Ty>;

    enum class rotation {
        no_rotation,
        left_right,
        all_around
    };

    class target_impl;

    class target {
    public:
        target(runtime *rt);
        ~target();

        void request_redraw();
        void broadcast(const std::string &name);
        std::vector<int> broadcast_and_wait(const std::string &name);
        bool check_waiting_threads(const std::vector<int> &waiting);
        
        void load_costume(const std::string &name, const std::string &path, int resolution = 1);
        void load_sound(const std::string &name, const std::string &path);

        void set_costume(int x);

        target_impl *get_impl() { return this->impl; }
    public:
        runtime *runtime;

        int x = 0;
        int y = 0;
        int direction = 90;
        int size = 100;
        bool visible = true;
        int current_costume = 0;
        rotation rotation_style = rotation::all_around;
        bool draggable = false;
        int tempo = 0;
        int volume = 100;

        bool need_redraw = false;
    protected:
        target_impl *impl;

        std::vector<std::string> costume_map;
        std::vector<std::string> sound_map;
    };

    class stage: public target {
    public:
        stage(shiro::runtime *rt);
        ~stage();
    };

    class sprite: public target {
    public:
        sprite(shiro::runtime *rt);
        ~sprite();

        // blocks
        void say(const std::string &str);
        template <arithmetic Ty> void say(const Ty &val) {
            this->say(std::to_string(val));
        }
    public:
    };
}

#endif // !_TARGET_H_
