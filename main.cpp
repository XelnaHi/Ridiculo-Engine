#define GLFW_INCLUDE_NONE // Disallow older versions of the OpenGL header to be included
#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

static constexpr int SCR_WIDTH = 800;
static constexpr int SCR_HEIGHT = 600;

void error_callback(int error, const char *description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

bool roofColorShouldChange = false;

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ridiculo Engine", NULL, NULL);
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

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float houseVertices[] = {
        1.f, 1.f, 0.0f, 1.0f, 0.f, 0.f, // top right
        1.f, 0.f, 0.0f, 1.0f, 0.f, 0.f, // bottom right
        -1.f, 0.f, 0.0f, 1.0f, 0.f, 0.f, // bottom left
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // From LearnOpenGL: note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // Question: but why would I want to unbind it?
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Rate limits the time between front- and back buffer swaps, and reduces screen tearing. Also known as VSync.
    glfwSwapInterval(1);

    Shader houseShader("shaders/2DHouse/vHouse.glsl", "shaders/2DHouse/fHouse.glsl");

    while (!glfwWindowShouldClose(window)) {
        // input is currently registered as a callback as part of the programs init phase

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        houseShader.use();
        glBindVertexArray(VAO);

        // Red roof
        if (roofColorShouldChange) {
            houseShader.drawTriangle(0.5f, 0.3f, 0.f, 0.3f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f);
        }
        // Green roof
        else {
            houseShader.drawTriangle(0.5f, 0.3f, 0.f, 0.3f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f);
        }

        // Walls
        houseShader.drawRect(0.4f, 0.8f, 0.f, 0.8f, 0.f, 0.f, 0.f, -0.5, 0.f);

        // Door
        houseShader.drawRect(0.1f, 0.3f, 0.f, 0.3f, 0.f, 0.f, 1.f, -0.5, 0.f);

        // Left window
        houseShader.drawRect(0.05f, 0.3f, 0.f, 0.3f, 0.f, 0.f, 1.f, -0.2, -0.3f);

        // Right window
        houseShader.drawRect(0.05f, 0.3f, 0.f, 0.3f, 0.f, 0.f, 1.f, -0.2, 0.3f);


        // Usually, all render commands go to the back buffer, while the front buffer contains the final output image.
        // Since rendering isn't instant, but rather pixel by pixel, only rendering to a single buffer would cause those leftover render artifacts to be visible.
        // After all rendering commands are done, we swap the buffers.
        glfwSwapBuffers(window);

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

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        roofColorShouldChange = !roofColorShouldChange;
    }
}
