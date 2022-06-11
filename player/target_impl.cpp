#include "target_impl.h"

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../vm/target.h"

namespace clipcc {

target_impl::target_impl(target *parent) {
    this->parent = parent;
    this->need_redraw = true;
}

target_impl::~target_impl() {
    // remove all textures
    for (auto &texture : this->textures) {
        texture.destroy();
    }
}

void target_impl::load_costume(const std::string &path) {
    this->load_texture(path);
}

void target_impl::set_costume(int x) {
    this->set_texture(x - 1);
}

void target_impl::request_redraw() {
    this->need_redraw = true;
}

int target_impl::load_texture(const std::string &path) {
    int width, height, channel_count;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channel_count, 4);

    if (!data) {
        //std::cerr << "[error] failed to load image" << std::endl;
        return -1;
    }

    texture new_texture(data, width, height);
    this->textures.push_back(new_texture);

    stbi_image_free(data);

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
    /*glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians<float>(90 - parent->direction), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 trans = glm::translate(rot, glm::vec3(parent->x, parent->y, 0.0f));
    this->model = trans * glm::scale(
        glm::mat4(1.0f),
        glm::vec3(
            this->current_texture->width * (parent->size / 100.0),
            this->current_texture->height * (parent->size / 100.0),
            1.0f
        )
    );*/
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(parent->x, parent->y, 0.0f));
    trans = glm::rotate(trans, glm::radians<float>(90 - parent->direction), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(
        this->current_texture->width * (parent->size / 100.0),
        this->current_texture->height * (parent->size / 100.0),
        1.0f
    ));
    this->model = trans;
}

}
