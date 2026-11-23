//
// Created by teo95 on 2026-11-17.
//

#include "Camera.h"

#include <iostream>

#include "../Config/Config.h"
#include "../GameState/GameState.h"
#include "../Renderer/Renderer.h"
#include "glm/gtc/type_ptr.hpp"

bool Camera::isPerspective = false;
bool Camera::isProjectionDirty = true;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      TurnSpeed(TURNSPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// The purpose of the lookat matrix is to convert positions related to a world origin to coordinates related
// to the camera as the origin. The lookat needs arguments for position, target and an "arbitrary" up vector. It returns
// the inverse of the cameras transform, converting world space to view space.
glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::UpdateProjectionMatrix() {
    int viewportWidth = Config::SCR_WIDTH / GameState::players.size();
    glm::mat4 projection = glm::mat4(1.0f);
    // Checks if we have pressed the keybind indicating that we want to switch projection modes
    if (isProjectionDirty) {

        if (!isPerspective) {
            projection = glm::perspective(glm::radians(45.0f), (float) viewportWidth / Config::SCR_HEIGHT, 0.1f,
                                          100.0f);
        } else {
            float aspect = static_cast<float>(viewportWidth) / static_cast<float>(Config::SCR_HEIGHT);
            float orthoHeight = 2.0f;
            float orthoWidth = orthoHeight * aspect;
            projection = glm::ortho(-orthoWidth / 2, orthoWidth / 2, -orthoHeight / 2, orthoHeight / 2, 0.1f,
                                    100.0f);
        }

        // Set the projection mode for all shaders. This is unlikely to change dynamically, so we want to refrain from setting this value over and
        // over unless necessary, thus we gate it behind the above if statement.
        for (auto &[name, shader]: Shader::shaderMap) {
            shader.use();
            // Apply projection for all shaders
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_projection"),
                               1, GL_FALSE, glm::value_ptr(projection));
        }

        // Our input has been processed and is no longer considered "dirty" (dirty meaning we have a desired change that has not yet
        // been processed
        isProjectionDirty = false;
    }
}

void Camera::Move(const float z, const float deltaTime) {
    // This magic number is supposed to be 'Speed' * 'deltaTime'. Needs to be refactored.
    Position += Front * z * (MovementSpeed * deltaTime);
}

void Camera::Rotate(float yaw, float pitch, const float deltaTime) {
    Yaw += yaw * TurnSpeed * deltaTime;
    Pitch += pitch * TurnSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped,
    // which happens if pitch becomes >= 90.0f || <= -90.0f
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    Zoom -= yOffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    // Safety thingy to normalize the front, but the above calculation already produces a unit vector. This could be
    // skipped but I like safety and explicitness so keeping it.
    Front = glm::normalize(front);

    // The Right vector value grows and shrinks depending on our cameras orientation, which is why we need to
    // normalize it.
    Right = glm::normalize(glm::cross(Front, WorldUp));

    // normalize the vectors, because their length gets closer to 0 the more you look up or down
    // which results in slower movement depending on orientation.
    Up = glm::normalize(glm::cross(Right, Front));
}
