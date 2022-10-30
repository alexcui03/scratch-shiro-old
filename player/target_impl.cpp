#include "target_impl.h"

#include <iostream>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <resvg.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../vm/target.h"

namespace shiro {

target_impl::target_impl(target *parent) {
    this->parent = parent;

    // init resvg
    resvg_init_log();
    this->resvg_option = resvg_options_create();
    resvg_options_load_system_fonts(this->resvg_option);
}

target_impl::~target_impl() {
    // remove all textures
    for (auto &texture : this->textures) {
        texture.destroy();
    }

    // uninit resvg
    resvg_options_destroy(this->resvg_option);
}

void target_impl::load_costume(const std::string &path, int resolution) {
    if (path.ends_with(".svg")) {
        this->load_vector_texture(path);
    }
    else {
        this->load_bitmap_texture(path, resolution);
    }
}

void target_impl::set_costume(int x) {
    this->set_texture(x);
}

int target_impl::load_bitmap_texture(const std::string &path, int resolution) {
    int width, height, channel_count;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channel_count, 4);

    if (!data) {
        std::cerr << "[error] failed to load bitmap image " << path << std::endl;
        return -1;
    }

    texture new_texture(data, width, height, resolution);
    this->textures.push_back(new_texture);

    stbi_image_free(data);

    return new_texture.id;
}

int target_impl::load_vector_texture(const std::string &path) {
    resvg_render_tree *render_tree;
    if (resvg_parse_tree_from_file(path.c_str(), this->resvg_option, &render_tree) != RESVG_OK) {
        std::cerr << "[error] failed to load vector image " << path << std::endl;
        return -1;
    }

    resvg_size size = resvg_get_image_size(render_tree);
    const int width = static_cast<int>(size.width);
    const int height = static_cast<int>(size.height);

    unsigned char *data = new unsigned char[width * height * 4];
    resvg_fit_to fit_to = { RESVG_FIT_TO_TYPE_ORIGINAL, 1 };
    resvg_render(render_tree, fit_to, resvg_transform_identity(), width, height, reinterpret_cast<char *>(data));
    resvg_tree_destroy(render_tree);

    texture new_texture(data, width, height);
    this->textures.push_back(new_texture);
    delete[] data;

    return new_texture.id;
}

void target_impl::remove_texture(unsigned int id) {
    glDeleteTextures(1, &id);
    this->textures.erase(std::remove_if(
        this->textures.begin(), this->textures.end(),
        [&](const texture &t) -> bool { return t.id == id; }
    ));
}

unsigned int target_impl::set_texture(unsigned int index) {
    this->current_texture = &this->textures[index];
    this->update_model();
    return this->current_texture->id;
}

void target_impl::update_model() {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(parent->x, parent->y, 0.0f));
    trans = glm::rotate(trans, glm::radians<float>(90 - parent->direction), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(
        this->current_texture->width * (static_cast<float>(parent->size) / this->current_texture->resolution / 100),
        this->current_texture->height * (static_cast<float>(parent->size) / this->current_texture->resolution / 100),
        1.0f
    ));
    this->model = trans;
    this->parent->need_redraw = false;
}

}
