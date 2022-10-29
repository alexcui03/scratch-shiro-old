#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace shiro {

static inline void check_shader_compile(int shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "[error] failed to compile shader\n" << log << std::endl;
    }
}

static inline void check_program_link(int program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(program, 512, nullptr, log);
        std::cerr << "[error] failed to link program\n" << log << std::endl;
    }
}

static std::string read_file(const std::string &path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "[error] failed to read file " << path << std::endl;
        return "";
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    fin.close();
    return stream.str();
}

shader::shader(const std::string &vertex_path, const std::string &fragment_path) {
    std::string &&vertex_code = read_file(vertex_path);
    const char *vertex_code_cstr = vertex_code.c_str();
    const int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_code_cstr, nullptr);
    glCompileShader(vertex_shader);
    check_shader_compile(vertex_shader);

    std::string &&fragment_code = read_file(fragment_path);
    const char *fragment_code_cstr = fragment_code.c_str();
    const int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_code_cstr, nullptr);
    glCompileShader(fragment_shader);
    check_shader_compile(fragment_shader);

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);
    glLinkProgram(this->id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

shader::~shader() {
    
}

void shader::use() {
    glUseProgram(this->id);
}

void shader::destroy() {
    glDeleteShader(this->id);
}

void shader::set_uniform1i(const char *name, int value) {
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void shader::set_uniform1ui(const char *name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(this->id, name), value);
}

void shader::set_uniform1f(const char *name, float value) {
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

}
