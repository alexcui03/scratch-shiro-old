#include <vm/clipcc_vm.h>

class target_0 : public clipcc::stage{
public:
    target_0(clipcc::runtime *rt) : clipcc::stage(rt) {
        this->load_costume("background1", "assets/0.png");
        this->set_costume(1);
        this->size = 50;
    }
    ~target_0() {}
};

class target_1 : public clipcc::sprite {
public:
    target_0 *stage;
    target_1(clipcc::runtime *rt, target_0 *t) : clipcc::sprite(rt), stage(t) {
        runtime->add_target(this, "sprite1");
        this->load_costume("costume1", "assets/1.png");
        this->set_costume(1);
        this->size = 10;
    }
    ~target_1() {}
    clipcc::coroutine func_0() {
        this->x = 0;
        this->y = 100;
        request_redraw();
        this->direction = 90;
        request_redraw();
        while (true) {
            {
                int steps = 30;
                double theta = clipcc::rad(90 - this->direction);
                this->x += std::cos(theta) * steps;
                this->y += std::sin(theta) * steps;
                request_redraw();
            }
            this->direction += 15;
            request_redraw();
            co_await std::suspend_always{};
        }
        co_return;
    }
};

clipcc::runtime *render_init() {
    clipcc::runtime *runtime = new clipcc::runtime;
    target_0 *target_0 = new ::target_0(runtime);
    target_1 *target_1 = new ::target_1(runtime, target_0);
    runtime->push_thread(new clipcc::thread(std::bind(&target_1::func_0, target_1)));
    return runtime;
}

extern int render_main();

int main() {
    return render_main();
}
