#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vm/shiro_vm.h>

#include "renderer.h"

// the external scratch_init function, which should be linked when compiling
extern shiro::runtime *scratch_init();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef PLATFORM_MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // PLATFORM_MACOS

    GLFWwindow *window = glfwCreateWindow(480, 360, "Scratch Player", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[error] failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "[error] failed to initialize glad" << std::endl;
        return -1;
    }

    shiro::runtime *runtime = scratch_init();

    shiro::renderer render(runtime);
    render.set_screen_size(480, 360);

    int framerate = 30;
    double last_frame = 0, current_frame = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        // control fps
        current_frame = glfwGetTime() * 1000;
        if (current_frame - last_frame >= 1000.0 / framerate) {
            last_frame = current_frame;
            runtime->excute();
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
