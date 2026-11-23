#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


// Class that processes input and calculates the corresponding Euler Angles,
// vectors and matrices.
class Camera {
public:
    static bool isPerspective;
    static bool isProjectionDirty;

    // Default camera values
    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;
    static constexpr float SPEED = 2.5f;
    static constexpr float TURNSPEED = 2.5f;
    static constexpr float SENSITIVITY = 0.1f;
    static constexpr float ZOOM = 45.0f;

    // Per Camera instance camera values
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float TurnSpeed;
    float MouseSensitivity;
    float Zoom;

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;


    // Construct camera class object using vectors. Learnopengl also implements a constructor
    // taking scalar values, but I don't see the point of it yet, so I'm not implementing it.
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    void UpdateProjectionMatrix();

    void Move(float z, float deltaTime);

    void Rotate(float yaw, float pitch, float deltaTime);

    /*
     * Probably want to refactor keyboard and mouse processing to a Controller class, and route input based
     * on a Controller ID.
     */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif
