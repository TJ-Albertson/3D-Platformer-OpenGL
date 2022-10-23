#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    camUP,
    camDOWN,
    camLEFT,
    camRIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.5f;
const float SENSITIVITY =  0.01f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 PlayerPosition;
    glm::vec3 CameraPosition;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, PlayerPosition, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 GetPlayerPosition()
    {
        return PlayerPosition;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            PlayerPosition.x -= CameraPosition.x * velocity;
            PlayerPosition.z -= CameraPosition.z * velocity;
        if (direction == BACKWARD)
            PlayerPosition += glm::vec3(-0.1f, 0.0f, 0.0f);
        if (direction == LEFT)
            PlayerPosition += glm::vec3(0.0f, 0.0f, -0.1f);
        if (direction == RIGHT)
            PlayerPosition += glm::vec3(0.0f, 0.0f, 0.1f);

        updateCameraVectors();
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch >= 1) {
            Pitch = 1;
        }
        if (Pitch <= -1) {
            Pitch = -1;
        }
       

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
       

        CameraPosition.x = 4 * sin(Yaw);
        CameraPosition.y = 4 * sin(Pitch);
        CameraPosition.z = 4 * cos(Yaw);

            /*
        glm::float32 x = 4 * sin(Yaw);
        glm::float32 z = 4 * cos(Yaw);

        glm::float32 y = 4 * sin(Pitch);
        */
        
        //std::cout << "X: " << x << std::endl;
        //std::cout << "Z: " << z << std::endl;

        Position = PlayerPosition + CameraPosition;
    }
};
#endif