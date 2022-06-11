#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "texture.h"

#include "../vm/target.h"

namespace clipcc {

class target_impl {
public:
    target_impl(target *parent);
    ~target_impl();
    
    // interface
    void load_costume(const std::string &path);
    void set_costume(int x);
    void request_redraw();

    bool should_redraw() { return this->need_redraw; }
    int load_texture(const std::string &path);
    void remove_texture(unsigned int id);
    inline unsigned int get_texture() { return this->current_texture->id; }
    unsigned int set_texture(unsigned int index);
    inline glm::mat4 get_model_matrix() { return this->model; }
    void update_model();
protected:
    target *parent;

    bool need_redraw;

    texture *current_texture;
    std::vector<texture> textures;
    glm::mat4 model = glm::mat4(1.0f);
};

}
