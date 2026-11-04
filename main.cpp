#define GLFW_INCLUDE_NONE // Disallow older versions of the OpenGL header to be included
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb/stb_image.h"

static constexpr int SCR_WIDTH = 800;
static constexpr int SCR_HEIGHT = 600;

void error_callback(int error, const char* description);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char* message, const void* userParam);
void generateSortofSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, int rings, int slices);

bool roofColorShouldChange = false;

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ridiculo Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float houseVertices[] = {
        //positions         // colors           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    float pyramidVertices[] = {
        //positon               //texture
                // South Face (front)
        0.0f, 0.5f, 0.0f,       0.5f, 1.0f, // top middle
        -0.5f, 0.0f, 0.5f,      0.0f, 0.0f, // bottom left
        0.5f, 0.0f, 0.5f,       1.0f, 0.0f, // bottom right

                // West Face (left)
        -0.5f, 0.0f, -0.5f,     1.0f, 0.0f, // bottom left

                // North Face (back)
        0.5f, 0.0f, -0.5f,     0.0f, 0.0f, // bottom left

                // East Face (right)
        0.5f, 0.0f, 0.5f,     1.0f, 0.0f, // bottom left

                // Floor Left
        -0.5f, 0.0f, -0.5f,       0.5f, 1.0f, // top left
        -0.5f, 0.0f, 0.5f,      0.0f, 1.0f, // bottom left
         0.5f, 0.0f, 0.5f,     0.0f, 0.0f, // bottom right

                // Floor Right
        -0.5f, 0.0f, -0.5f,       0.5f, 1.0f, // top left
        0.5f, 0.0f, -0.5f,      0.0f, 1.0f, // top right
         0.5f, 0.0f, 0.5f,     0.0f, 0.0f, // bottom right
    };

    unsigned int pyramidIndices[] = {
       0, 1, 2, // South Face
       0, 1, 3, // West Face
       0, 3, 4, // North Face
       0, 4, 5, // East Face
       3, 1, 5, // Floor Left Face
       3, 4, 2, // Floor Right Face
    };

    float cubeVertices[] = {
        // position             // texture
                    // South Left T
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.5f,      0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // South Right T
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, // top left
        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // West Left T
        -0.5f, 0.5f, -0.5f,      0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,      0.0f, 0.0f, // bottom left
        -0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // West Right T
        -0.5f, 0.5f, -0.5f,      0.0f, 1.0f, // top left
        -0.5f, 0.5f, 0.5f,       1.0f, 1.0f, // top right
        -0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // North Left T
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, // bottom left
        0.5f, -0.5f, -0.5f,      1.0f, 0.0f, // bottom right

                    // North Right T
        -0.5f, 0.5f, -0.5f,      0.0f, 1.0f, // top left
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, -0.5f, -0.5f,      1.0f, 0.0f, // bottom right

                    // East Left T
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f, // top left
        0.5f, -0.5f, 0.5f,       0.0f, 0.0f, // bottom left
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom right

                    // East Right T
        0.5f, 0.5f, 0.5f,       0.0f, 1.0f, // top left
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom right

                    // Top Left T
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, // top left
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, // bottom left
        0.5f, 0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // Top Right T
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, // top left
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, 0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // Bottom Left T
        -0.5f, -0.5f, -0.5f,     0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.5f,      0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right

                    // Bottom Right T
        -0.5f, -0.5f, -0.5f,     0.0f, 1.0f, // top left
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, // bottom right


    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("images/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // From LearnOpenGL: note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // Question: but why would I want to unbind it?
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    unsigned int VAO_TWO, VBO_TWO, EBO_TWO;
    glGenVertexArrays(1, &VAO_TWO);
    glGenBuffers(1, &VBO_TWO);
    glBindVertexArray(VAO_TWO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_TWO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    std::vector<float> spherePositions;
    std::vector<unsigned int> sphereIndices;
    generateSortofSphere(spherePositions, sphereIndices, 16, 32);

    std::vector<float> sphereVertices; // interleaved: x y z u v
    for (size_t i = 0; i < spherePositions.size(); i += 3)
    {
        float x = spherePositions[i];
        float y = spherePositions[i + 1];
        float z = spherePositions[i + 2];

        float u = atan2f(z, x) / (2.0f * 3.14159265f) + 0.5f;
        float v = asinf(glm::clamp(y, -1.0f, 1.0f)) / 3.14159265f + 0.5f;

        sphereVertices.insert(sphereVertices.end(), { x, y, z, u, v });
    }

    unsigned int sphereVAO, sphereVBO, sphereEBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Rate limits the time between front- and back buffer swaps, and reduces screen tearing. Also known as VSync.
    glfwSwapInterval(1);

    Shader houseShader("shaders/2DHouse/vHouse.glsl", "shaders/2DHouse/fHouse.glsl");

    glEnable(GL_DEPTH_TEST); // Allows fragments z-values to be compared to the depth buffer, helping us decide whether
    // a fragment should be rendered or discarded. This state (enabled/disabled) is stored until otherwise specified.

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    while (!glfwWindowShouldClose(window))
    {
        // input is currently registered as a callback as part of the programs init phase

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Clears the previous buffer of it's color/depth values. If we dont, we risk using and old
        // buffers values.

        houseShader.use();
        glBindVertexArray(VAO);
        glUniform1i(glGetUniformLocation(houseShader.ID, "u_Texture"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        //
        int modelLoc = glGetUniformLocation(houseShader.ID, "u_model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //
        int viewLoc = glGetUniformLocation(houseShader.ID, "u_view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // // Projection rarely changes, which is why best practice is to set the uniform once outside the render loop.
        // // I'm keeping it here for now for quicker readability while practicing matrices.
        int projLoc = glGetUniformLocation(houseShader.ID, "u_projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO_TWO);
        glUniform1i(glGetUniformLocation(houseShader.ID, "u_Texture"), 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
        model = glm::scale(model, (float)sin(glfwGetTime()) * glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model,  glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        modelLoc = glGetUniformLocation(houseShader.ID, "u_model");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 72);

        glBindVertexArray(sphereVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, -0.7f, 0.0f));
        model = glm::translate(model,(float)sin(glfwGetTime()) * glm::vec3(-0.3f, 0.0, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        modelLoc = glGetUniformLocation(houseShader.ID, "u_model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        glDrawElements(GL_TRIANGLES, (GLsizei)sphereIndices.size(), GL_UNSIGNED_INT, 0);


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

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        roofColorShouldChange = !roofColorShouldChange;
    }
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
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

    switch (type)
    {
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

    switch (severity)
    {
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
