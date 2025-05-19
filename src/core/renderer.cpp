#include <pybind11/pybind11.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/3.4/include/GLFW/glfw3.h>

#include "public/FileSupport.h"
#include "public/Shader.h"
#include "public/Camera.h"
#include "public/Texture.h"
#include "public/Light.h"

namespace py = pybind11;

// 기본 화면 크기 설정. 이후 변경할 수 있도록 구현 예정.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 카메라의 방향, 위치 관련 관련 초기화 변수.
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 WHITELIGHT = glm::vec3(0.0f, 0.0f, 0.0f);

// 카메라의 조작과 관련된 초기 변수,
bool firstMouse = true;
float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

// DeltaTime을 사용하기 위한 변수.
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera viewportCamera(cameraPos);

// process all input: 해당 프레임에서 glfw에 관련된 키가 입력됐는지 확인하고, 쿼리한다.
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, RIGHT);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, UP);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, DOWN);
}

// glfw: 윈도우 크기 변경 시 해당 함수 호출.
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // 뷰포트와 새창 크기의 일치를 확인.
    glViewport(0, 0, width, height);
}

// 마우스 이동시 콜백되는 함수.
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    // 카메라가 뒤집어지지 않도록 값을 clamp
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    viewportCamera.cameraRotate(xoffset, yoffset);
}

// 마우스의 휠을 굴리면 콜백되는 함수.
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    viewportCamera.cameraScroll(static_cast<float>(yoffset));
}

void calcDeltaTime()
{
    float time = glfwGetTime();
    deltaTime = time - lastFrame;
    lastFrame = time;
}

class Renderer
{
public:
    Renderer()
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
    }

    ~Renderer()
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    void render_frame()
    { // deltatime 구하기.
        calcDeltaTime();

        // 입력 콜백 확인.
        processInput(window);
        IsClose(window);

        // 배경 렌더링 및 렌더링 시작.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(viewportCamera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = viewportCamera.getCameraViewMat();

        glm::mat4 model = glm::mat4(1.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool IsClose(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

private:
    GLFWwindow *window;
};

PYBIND11_MODULE(cpp_renderer, m)
{
    py::class_<Renderer>(m, "Renderer")
        .def(py::init<>())
        .def("render_frame", &Renderer::render_frame)
        .def("isclose", &Renderer::IsClose);
}
