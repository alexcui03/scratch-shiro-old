// This is generated by clipcc-compiler.
#include <vm/clipcc_vm.h>

class target_0: public clipcc::stage {
public:
target_0(clipcc::runtime *rt): clipcc::stage(rt) {
this->load_costume("backdrop1", "assets/c446646a95cd43c36d25583fdaea3dbc.png");
this->x = 0;
this->y = 0;
this->direction = 0;
this->size = 50;
this->visible = false;
this->set_costume(1);
}
~target_0() {
}
};
class target_1: public clipcc::sprite {
public:
target_0 *stage;
target_1(clipcc::runtime *rt, target_0 *t): clipcc::sprite(rt), stage(t) {
runtime->add_target(this, "nmsl");
this->load_costume("smile", "assets/73119304cc438c543d3f4fd593671637.png");
this->load_costume("ww", "assets/6fd8e277b79dfb67e7c6022f99809a37.png");
this->load_costume("unhappy", "assets/4f1bb9a16d8f9a65e4b75c4be3c9981a.png");
this->load_costume("angry", "assets/5140891add0a268266a78c54eb631d87.png");
this->x = 128;
this->y = 1;
this->direction = 180;
this->size = 50;
this->visible = true;
this->set_costume(1);
}
~target_1() {
}
clipcc::coroutine func_0() {
this->x = 0; this->y = 100; request_redraw();
this->direction = 90; request_redraw();
while (true) {
{ int steps = 30; double theta = clipcc::rad(90 - this->direction); this->x += std::cos(theta) * steps; this->y += std::sin(theta) * steps; request_redraw(); }this->direction += 15; request_redraw();co_await std::suspend_always{};
}
co_return;
}
};
clipcc::runtime *scratch_init() {
clipcc::runtime *runtime = new clipcc::runtime;
target_0 *target_0 = new ::target_0(runtime);
target_1 *target_1 = new ::target_1(runtime, target_0);
runtime->push_thread(new clipcc::thread(std::bind(&target_1::func_0, target_1)));

return runtime;
}