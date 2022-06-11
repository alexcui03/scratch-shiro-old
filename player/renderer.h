#pragma once

#include <glm/glm.hpp>

#include "../vm/runtime.h"

#include "target_impl.h"
#include "shader.h"

namespace clipcc {

class renderer {
public:
    renderer(runtime *rt);
    ~renderer();
    void set_screen_size(unsigned int width, unsigned int height);
    void set_stage(target_impl *obj);
    void add_sprite(target_impl *obj);
    void render();
private:
    unsigned int vao;
    unsigned int vbo;

    shader *shader;
    target_impl *stage;
    std::vector<target_impl *> sprites;

    unsigned int screen_width = 480;
    unsigned int screen_height = 360;

    glm::mat4 projection;
};

}
