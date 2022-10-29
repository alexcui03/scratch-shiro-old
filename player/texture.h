#pragma once

namespace shiro {

class texture {
public:
    texture(unsigned char *data, unsigned int width, unsigned int height, unsigned int resolution = 1);
    ~texture();
    void destroy();
public:
    unsigned int id;
    unsigned int width;
    unsigned int height;
    unsigned int resolution;
};

}

