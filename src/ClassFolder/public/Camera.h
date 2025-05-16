#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum cameraMoveDirection
{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    //카메라 attribute, 
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // 오일러 각도 표현용 데이터'
    float Yaw;
    float Pitch;

    // 카메라 옵션
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

public:
    Camera(glm::vec3 initPos);

    void CreateCamera(const glm::vec3 initPos);
    void cameraMove(const float deltaTime, cameraMoveDirection moveDir);
    void cameraRotate(float x, float y, GLboolean constraintPitch = true);
    void cameraScroll(float y);
    void cameraSensitive(const float y);

    glm::mat4 getCameraViewMat();

    void updateCamera();
};

#endif