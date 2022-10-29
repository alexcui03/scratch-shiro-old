#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../vm/runtime.h"

namespace shiro {

#define offset(x, type) reinterpret_cast<void*>((x) * sizeof(type))

static float vertices[] = {
    //  x,     y,     z,     s,     t
     0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.0f,  0.0f,  1.0f
};

renderer::renderer(runtime *rt) {
    this->stage = rt->stage->get_impl();
    for (auto &target : rt->target_map) {
        this->sprites.push_back(target.second->get_impl());
    }

    // init shader
    this->shader = new shiro::shader("shader/target.vert", "shader/target.frag");

    // init vao and vbo
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // model pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), offset(0, float));
    glEnableVertexAttribArray(0);
    
    // texture pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), offset(3, float));
    glEnableVertexAttribArray(1);

    this->projection = glm::ortho<float>(
        -(this->screen_width / 2.0), this->screen_width / 2.0,
        -(this->screen_height / 2.0), this->screen_height / 2.0,
        0.0f, 100.0f
    );
}

renderer::~renderer() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);

    this->shader->destroy();
}

void renderer::set_screen_size(unsigned int width, unsigned int height) {
    this->screen_width = width;
    this->screen_height = height;
    
    this->projection = glm::ortho<float>(
        -(this->screen_width / 2.0), this->screen_width / 2.0,
        -(this->screen_height / 2.0), this->screen_height / 2.0,
        0.0f, 100.0f
    );
}

void renderer::set_stage(target_impl *obj) {
    this->stage = obj;
}

void renderer::add_sprite(target_impl *obj) {
    this->sprites.push_back(obj);
}

void renderer::render() {
    this->shader->use();
    glUniformMatrix4fv(
        glGetUniformLocation(this->shader->id, "projection"),
        1, GL_FALSE, glm::value_ptr(this->projection)
    );

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->vao);

    // render stage
    if (this->stage->should_redraw()) this->stage->update_model();
    glBindTexture(GL_TEXTURE_2D, this->stage->get_texture());
    glUniformMatrix4fv(
        glGetUniformLocation(this->shader->id, "model"),
        1, GL_FALSE, glm::value_ptr(this->stage->get_model_matrix())
    );
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render all sprites
    for (auto &sprite : this->sprites) {
        if (sprite->should_redraw()) sprite->update_model();
        glBindTexture(GL_TEXTURE_2D, sprite->get_texture());
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader->id, "model"),
            1, GL_FALSE, glm::value_ptr(sprite->get_model_matrix())
        );
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

}
