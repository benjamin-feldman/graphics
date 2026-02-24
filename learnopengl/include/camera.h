#ifndef CAMERA_H
#define CAMERA_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <iostream>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;
    float fov;

    float movementSpeed;
    float mouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f))
        : position(position),
          front(glm::vec3(0.0f, 0.0f, -1.0f)),
          up(glm::vec3(0.0f, 1.0f, 0.0f)),
          yaw(-90.0f),
          pitch(0.0f),
          fov(45.0f),
          movementSpeed(2.5f),
          mouseSensitivity(0.1f)
    {}

    glm::mat4 GetViewMatrix() {
        // return glm::lookAt(position, position + front, up);
        // Custom (inefficient) implementation of the lookAt method
        glm::vec3 direction = front;
        glm::vec3 cameraZ = - glm::normalize(direction);
        glm::vec3 cameraX = glm::normalize(glm::cross(up, cameraZ));
        glm::vec3 cameraY = glm::normalize(glm::cross(cameraZ, cameraX));

        glm::mat4 rotationCameraToWorld = glm::mat4(
            glm::vec4(cameraX, 0.0f),
            glm::vec4(cameraY, 0.0f),
            glm::vec4(cameraZ, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        glm::mat4 lookAtRotation = glm::transpose(rotationCameraToWorld);
        glm::mat4 lookAtTranslation = glm::mat4(1.0f);
        lookAtTranslation[3] = glm::vec4(-position.x, -position.y, -position.z, 1.0f);

        return lookAtRotation * lookAtTranslation;
    }

    void ProcessKeyboard(int direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == 0) position += front * velocity;
        if (direction == 1) position -= front * velocity;
        if (direction == 2) position -= glm::normalize(glm::cross(front, up)) * velocity;
        if (direction == 3) position += glm::normalize(glm::cross(front, up)) * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }

    void ProcessMouseScroll(float yoffset) {
        fov -= yoffset;
        if (fov < 1.0f) fov = 1.0f;
        if (fov > 45.0f) fov = 45.0f;
    }
};

#endif
