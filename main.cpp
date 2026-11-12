#define GLFW_INCLUDE_NONE // Disallow older versions of the OpenGL header to be included
#include <cmath>
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
#include "custom_obj_loader/CustomMesh.h"
#include "custom_obj_loader/OBJLoader.h"
#include "model_loader/Model.h"
#include "stb/stb_image.h"


static constexpr int SCR_WIDTH = 800;
static constexpr int SCR_HEIGHT = 600;

void error_callback(int error, const char *description);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char *message, const void *userParam);

bool roofColorShouldChange = false;

glm::mat4 projection = glm::mat4(1.0f);
bool isPerspective = false;
bool isProjectionDirty = true;


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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);


    // Rate limits the time between front- and back buffer swaps, and reduces screen tearing. Also known as VSync.
    glfwSwapInterval(1);

    stbi_set_flip_vertically_on_load(true);
    Shader houseShader("shaders/2DHouse/vHouse.glsl", "shaders/2DHouse/fHouse.glsl");
    Shader suzanneShader("shaders/Week3/vSuzanne.glsl", "shaders/Week3/fSuzanne.glsl");

    glEnable(GL_DEPTH_TEST); // Allows fragments z-values to be compared to the depth buffer, helping us decide whether
    // a fragment should be rendered or discarded. This state (enabled/disabled) is stored until otherwise specified.

    // Research cull face
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Model backpackModel("models/backpack/backpack.obj");

    CustomMesh* suzanne = OBJLoader::LoadMesh("models/suzanne/Suzanne.obj");

    while (!glfwWindowShouldClose(window)) {
        // input is currently registered as a callback as part of the programs init phase

        // Set the color to use when clearing the frame buffers.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clears the previous buffer of it's color/depth values. If we dont, we                                                            risk using old buffers values.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        suzanneShader.use();

        // Checks if we have pressed the keybind indicating that we want to switch projection modes
        if (isProjectionDirty) {
            if (!isPerspective) {
                projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
            } else {
                float aspect = (float) SCR_WIDTH / (float) SCR_HEIGHT;
                float orthoHeight = 2.0f;
                float orthoWidth = orthoHeight * aspect;
                projection = glm::ortho(-orthoWidth / 2, orthoWidth / 2, -orthoHeight / 2, orthoHeight / 2, 0.1f,
                                        100.0f);
            }

            // Set the projection mode. This is unlikely to change dynamically, so we want to refrain from setting this value over and
            // over unless necessary, thus we gate it behind the above if statement.
            glUniformMatrix4fv(glGetUniformLocation(suzanneShader.ID, "u_projection"),
                               1, GL_FALSE, glm::value_ptr(projection));

            // Our input has been processed and is no longer considered "dirty" (dirty meaning we have a desired change that has not yet
            // been processed
            isProjectionDirty = false;
        }

        // Caching the uniform locations. I would like to have this done pre-render-loop, but I'm not sure how. If the shader program
        // is switched to another, the values have to be reset anyway, so I'm not sure if my desired "optimization" is actually viable
        int modelLoc = glGetUniformLocation(suzanneShader.ID, "u_model");
        int viewLoc = glGetUniformLocation(suzanneShader.ID, "u_view");


        // Set the matrices to identity matrices in order to give them clean slates. If we dont, we risk using old matrix values
        // which can skew our transformation matrices as we start applying translation, rotation and scale.

        // Local space = coordinate space local to the object
        // World space = coordinates relative to a shared game world origin
        // View space = often referred to as the camera. This is the result of transforming world space coordinates into coordinates that are in front of the users view.
        // Clip space = refers to the space within which fragments are eligible for rendering. Coordinates outside of this range are not rendered, ie clipped. Working with coordinates within the range [-1, 1] is generally considered unintuitive which is why we use a projection matrix to specify an easier-to-work-with range for each axis, ie [600, 800] which the projection matrix then converts to NDC coordinates. Coordinates outside of this custom range (for example [650, 550]) is thus mapped to be outside of the range [-1,1] and is subsequently not rendered on screen (clipped). The viewing box that projection creates is called a frustum. Once all vertices have been mapped to clip space the vertex shader performs the perspective division, meaning it divides the x, y and z components by the vector's homogenous w component. After this, the resulting coordinates are mapped to screen coordinates by using the dimensions set in glViewport(width, height) which then gets turned into fragments.

        // Orthographic projection = a cube-like frustum defined by a near-plane, far-plane, width, height and length. Coordinates in front of the near plane, and coordinates behind the far plane are clipped. This type of projection doesn't touch the w component which means that if the w remains = 1.0f, x, y and z components will remain intact during perspective division. This projection mode is usually used for 2D rendering.

        // Perspective projection = we can roughly describe perspective as objects appearing smaller the further away they are from us, as well as objects appearing as if coinciding, or "travelling towards"  a shared point in the distance. Perspective projection mimics this by increasing the value of the w component the further away an object is from the origin. When perspective division happens, this means that x, y and z coordinates are scaled based on the value of the w component. Perspective projection defines a clip space within the range of [-w, w]. Coordinates outside of this range fall outside of the mapped-to NDC coordinates, and are thus not rendered.

        // Similarly to orthographic projection, perspective projection creates a frustum with a near and far plane, with the caveat that this frustum is non-uniform (whereas the ortho proj is uniformly cube-like) and includes a field of view specified in radians or degrees. Furthermore, it uses an aspect ratio equal to width divided by height.


        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        // This is essentially our camera. After having applied the model matrix, the view matrix is applied, translating all objects 4 units away from the "camera", creating a sense of distance.
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

        // IMPORTANT MENTAL NOTE!!
        // I keep mixing up application of rotation, scale and translation when multiplying a transformation matrix with a vector, and the act of *building the transformation itself*. Depending on what order you multiply the rotation, scale and translation matrices in order to build the transformation matrix does have an impact on the final result due to matrices non-commutative nature.

        // Multiplying matrices in the following order; I * T * R * S can then be read as S (scale) being applied to the vector first, then R (rotation) and then T (translation). We get different "behaviors" depending on what order we multiply these matrices when building them. If we were to to R * T, instead of T * R, we would get an orbiting result, because we would first translate the object away from the origin, and then rotate it around the origin as opposed to first rotating it around the origin, and then translating it away from the origin.

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Send matrix model value from CPU --> GPU model uniform
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Send view matrix value from CPU --> GPU view unifrom
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        suzanne->Render(suzanneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.5f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        suzanne->Render(suzanneShader);

        // backpackModel.draw(houseShader);


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

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        isPerspective = !isPerspective;
        isProjectionDirty = true;
    }
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
