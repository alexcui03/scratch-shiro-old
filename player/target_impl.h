#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <resvg.h>

#include "texture.h"

#include "../vm/target.h"

namespace shiro {

class target_impl {
public:
    target_impl(target *parent);
    ~target_impl();
    
    // interface
    void load_costume(const std::string &path, int resolution = 1);
    void set_costume(int x);

    inline bool should_redraw() { return this->parent->need_redraw; }
    int load_bitmap_texture(const std::string &path, int resolution = 1);
    int load_vector_texture(const std::string &path);
    void remove_texture(unsigned int id);
    inline unsigned int get_texture() { return this->current_texture->id; }
    unsigned int set_texture(unsigned int index);
    inline glm::mat4 get_model_matrix() { return this->model; }
    void update_model();
protected:
    target *parent;

    resvg_options *resvg_option;

    texture *current_texture;
    std::vector<texture> textures;
    glm::mat4 model = glm::mat4(1.0f);
};

}
