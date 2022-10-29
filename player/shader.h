#pragma once

#include <string>

namespace shiro {

class shader {
public:
    shader(const std::string &vertex_path, const std::string &fragment_path);
    ~shader();
    void use();
    void destroy();
    void set_uniform1i(const char *name, int value);
    void set_uniform1ui(const char *name, unsigned int value);
    void set_uniform1f(const char *name, float value);
public:
    unsigned int id;
};

}
