#include <glad/glad.h>
#include <glfw/3.4/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <public/Camera.h>

Camera::Camera(const glm::vec3 initPos)
{
    CreateCamera(initPos);
}

// 기본 값을 설정하는 카메라 생성 함수,
// 생성자에서 들어온 값을 사용해서 초기화를 진행한다.
void Camera::CreateCamera(const glm::vec3 initPos)
{
    glm::vec3 cameraPos = initPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    this->MovementSpeed = SPEED;
    this->Yaw = YAW;
    this->Pitch = PITCH;
    this->Zoom = ZOOM;
    this->Position = cameraPos;
    this->Front = cameraFront;
    this->Up = cameraUp;
    this->MouseSensitivity = SENSITIVITY;

    WorldUp = Up;

    updateCamera();
}

// 키보드 입력에 따라서 카메라를 이동
void Camera::cameraMove(const float deltaTime, cameraMoveDirection moveDir)
{
    float cameraSpeed = MovementSpeed * deltaTime;
    switch (moveDir)
    {
    case cameraMoveDirection::FORWARD:
    {
        Position += Front * cameraSpeed;
        break;
    }
    case cameraMoveDirection::BACKWARD:
    {
        Position -= Front * cameraSpeed;
        break;
    }
    case cameraMoveDirection::LEFT:
    {
        Position -= Right * cameraSpeed;
        break;
    }
    case cameraMoveDirection::RIGHT:
    {
        Position += Right * cameraSpeed;
        break;
    }
    case cameraMoveDirection::UP:
    {
        Position -= Up * cameraSpeed;
        break;
    }
    case cameraMoveDirection::DOWN:
    {
        Position += Up * cameraSpeed;
        break;
    }
    default:
        break;
    }

    updateCamera();
}

// 마우스 입력에 따라서 카메라를 회전.
void Camera::cameraRotate(float x, float y, GLboolean constraintPitch /* = true*/)
{
    x *= SENSITIVITY;
    y *= SENSITIVITY;
    
    Yaw += x;
    Pitch += y;
     
    if (Pitch > 89.0f)
    {
        Pitch = 89.0f;
    }
    if (Pitch < -89.0f)
    {
        Pitch = -89.0f;
    }

    updateCamera();
}

// 마우스 스크롤을 받아서, zoom을 설정하는 함수.
void Camera::cameraScroll(float y)
{
    updateCamera();
}

// 별도로 마우스 감도를 설정하는 함수.
void Camera::cameraSensitive(const float y)
{
}

glm::mat4 Camera::getCameraViewMat()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// 별도로 설정된 카메라 관련 데이터를 업데이트, 카메라를 이동시킨다.
void Camera::updateCamera()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    
    this->Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(Right, Front));
}
