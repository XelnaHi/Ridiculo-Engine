#define GLFW_INCLUDE_NONE // Disallow older versions of the OpenGL header to be included
#include <array>
#include <complex>
#include <filesystem>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "Shader.h"
#include "assimp/version.h"
#include "Camera/Camera.h"
#include "Controller/Controller.h"
#include "GameState/GameState.h"
#include "Levels/LevelManager.h"
#include "Renderer/Renderer.h"
#include "Player/Player.h"
#include "SceneParser/SceneParser.h"
#include "stb/stb_image.h"


static constexpr int SCR_WIDTH = 1920;
static constexpr int SCR_HEIGHT = 1080;

void error_callback(int error, const char *description);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);


void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char *message, const void *userParam);

void mouse_callback(GLFWwindow *window, double xPos, double yPos);


glm::mat4 projection = glm::mat4(1.0f);
bool isPerspective = false;
bool isProjectionDirty = true;


// Most notably used for making frame originally rate-dependant calculations, such as velocity calculations,
// non-frame rate dependant. In the example of velocity, this means that a machine with higher FPS
// won't gain a velocity advantage compared to a machine with lower FPS.
float deltaTime = 0.0f;
float lastFrame = 0.0f;


bool firstMouseScreenEnter = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastMouseX = SCR_WIDTH / 2.0f;
float lastMouseY = SCR_HEIGHT / 2.0f;

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Check that Assimp was included correctly
    Assimp::Importer importer;
    std::cout << "Assimp OK, version " << aiGetVersionMajor() << "." << aiGetVersionMinor() << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ridiculo Engine", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }


    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST); // Allows fragments z-values to be compared to the depth buffer, helping us decide whether
    // a fragment should be rendered or discarded. This state (enabled/disabled) is stored until otherwise specified.


    // Rate limits the time between front- and back buffer swaps, and reduces screen tearing. Also known as VSync.
    glfwSwapInterval(1);

    stbi_set_flip_vertically_on_load(true);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    Renderer::useCustomLoader = false;

    if (Renderer::useCustomLoader) {
        std::cout << "Using custom OBJ loader to load models." << std::endl;
    } else {
        std::cout << "Using Assimp to load models." << std::endl;
    }


    Renderer::InitShaders();

    Bindings bindings_p1 = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_Q, GLFW_KEY_E};
    Bindings bindings_p2 = {
        GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_PAGE_DOWN, GLFW_KEY_PAGE_UP
    };
    GameState::CreateController(window, bindings_p1);
    GameState::CreateController(window, bindings_p2);

    SceneParser::ParseSceneFile("Levels/sceneFormat");
    LevelManager::InitLevel(0);




    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear the input struct for all players every frame.
        for (unsigned int i = 0; i < GameState::playerInputs.size(); i++) {
            GameState::playerInputs[i] = {};
            GameState::playerInputs[i] = GameState::controllers[i].PollInput();
            GameState::controllers[i].player->update(GameState::playerInputs[i]);
        }

        // Set the color to use when clearing the frame buffers.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clears the previous buffer of its color/depth values. If we don't, we risk using old buffers values.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::Render();


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

/*
 * Called everytime the mouse moves. Used to register the x and y values of the cursor, which we use to calculate
 * pitch and yaw of the camera
 */
void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    if (firstMouseScreenEnter) {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstMouseScreenEnter = false;
    }

    float xOffset = static_cast<float>(xPos) - lastMouseX;
    float yOffset = lastMouseY - static_cast<float>(yPos); // yPos is in screen space, which has

    lastMouseX = xPos;
    lastMouseY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API: std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
