#define GLFW_INCLUDE_NONE // Disallow older versions of the OpenGL header to be included
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

void error_callback(int error, const char *description);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ridiculo Engine", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();

        glfwSwapInterval(1); // rate limits the time between front- and back buffer swaps, and reduces screen tearing. Also known as VSync.

        glfwSwapBuffers(window); // Usually, all render commands go to the back buffer, while the front buffer contains the final output image.
        // Since rendering isn't instant, but rather pixel by pixel, only rendering to a single buffer would cause those leftover render artifacts to be visible.
        // After all rendering commands are done, we swap the buffers.

        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
