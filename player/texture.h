#pragma once

namespace clipcc {

class texture {
public:
    texture(unsigned char *data, unsigned int width, unsigned int height);
    ~texture();
    void destroy();
public:
    unsigned int id;
    unsigned int width;
    unsigned int height;
};

}

