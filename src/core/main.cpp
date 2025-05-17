#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/3.4/include/GLFW/glfw3.h>
#include <iostream>

#include "Util/FileSupport.h"
#include "public/Shader.h"
#include "public/Camera.h"
#include "public/Texture.h"
#include "public/Light.h"

// 프레임 버퍼 함수 크기 및 프로세스 입력 함수 선언.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void calcDeltaTime();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 WHITELIGHT  = glm::vec3(0.0f, 0.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  SCR_WIDTH / 2.0;
float lastY =  SCR_HEIGHT / 2.0;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera viewportCamera(cameraPos);

int main()
{
    // glfw: 초기화, glfw를 초기화하고 내부 필요한 데이터를 초기화한다.
    // windowHint를 통해서 현재 glfw의 버전과 opengl의 core를 설정해 준다.
    // -----------------------------
    if (!glfwInit())
    {
        return -1;
    }
    glfwSetErrorCallback(
        [](int error, const char *errorDesc)
        {
            std::cerr << "GLFW ERROR : " << errorDesc << std::endl;
        });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 만약 애플이라면, 애플버전으로 한번 더 해준다.
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw윈도우를 생성한다.
    // Create Window를 통해서 opengl윈도우 생성 및 해상도 설정을 진행.
    // 실패한다면 glfw를 초기화하고 종료한다.
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    int currentMajorVersion, currentMinorVersion, currentRev;
    glfwGetVersion(&currentMajorVersion, &currentMinorVersion, &currentRev);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: 초기화.
    // glad에 현재 glfw의 프로세스 주소를 넘겨서 현재 glad가 glfw를 관리할수 있도록 해 준다.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //depth test enable
    glEnable(GL_DEPTH_TEST);

    // Shader코드를 가져와서 컴파일 이후 프로그램 id를 할당.
    std::string vertexShaderCodeStr;
    FileFromString("src/core/Shader/LitShader.vs", vertexShaderCodeStr);

    std::string fragmentShaderCodeStr;
    FileFromString("src/core/Shader/LitShader.fs", fragmentShaderCodeStr);

    std::string LightVSCodeStr;
    FileFromString("src/core/Shader/LightShader.vs", LightVSCodeStr);

    std::string LightFSCodeStr;
    FileFromString("src/core/Shader/LightShader.fs", LightFSCodeStr);

    Shader *shaderClass = new Shader(vertexShaderCodeStr.c_str(), fragmentShaderCodeStr.c_str());
    Light *lightShaderClass = new Light(LightVSCodeStr.c_str(), LightFSCodeStr.c_str(), WHITELIGHT);

  float vertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,   0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   0.0f,  1.0f,  0.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture textureObj;
    textureObj.CreateTexture("resource/Texture/container.png");
    textureObj.CreateTexture("resource/Texture/Emissive.png");
    textureObj.SetTexture(VAO, shaderClass->ID);
    
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightamb = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightspec = glm::vec3(1.0f, 1.0f, 1.0f);

    lightShaderClass->SetLightColor(lightColor);
    lightShaderClass->SetLightObjectColor(objColor);
    lightShaderClass->SetLightAmbient(lightamb);
    lightShaderClass->SetLightSpecular(lightspec);

    // render loop.
    // 현재 glfw window의 메인 루프를 진행한다.
    // 현재 윈도우가 켜져 있으면, 입력을 받고, 버퍼를 교체하고, 이벤트를 돌려준다.
    while (!glfwWindowShouldClose(window))
    {
        //deltatime 구하기.
        calcDeltaTime();

        // 입력 콜백 확인.
        processInput(window);

        // 배경 렌더링 및 렌더링 시작.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(viewportCamera.Zoom), 
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = viewportCamera.getCameraViewMat();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        
        lightShaderClass->DrawLightObject(projection, view, model);

        // render boxes
        for (unsigned int i = 0; i < 10; i++)
        {
            shaderClass->use();
            shaderClass->setMat4("projection", projection);
            shaderClass->setMat4("view", view);

            // 여러개의 객체를 그림에 따라서, 기본 값 행렬을 일단 생성함.
            // 행렬의 값이 1인 기본 행렬을 생성한 뒤, 별도의 값을 통해서 행렬을 수정.
            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::translate(model, cubePositions[i]);

            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            glm::vec3 amb = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 dif = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 spec = glm::vec3( 0.5f, 0.5f, 0.5f);
            float shn = 32.0f;

            shaderClass->setVec3("material.ambient", amb);
            shaderClass->setVec3("material.diffuse", dif);
            shaderClass->setVec3("material.specular", spec);
            shaderClass->setFloat("material.shininess", shn);

            shaderClass->setVec3("light.ambient", lightShaderClass->lightAmbient);
            shaderClass->setVec3("light.diffuse", lightShaderClass->lightColor); // Scene에 맞는 어두운 빛
            shaderClass->setVec3("light.specular", lightShaderClass->lightSpecular);

            shaderClass->setMat4("model", model);
            shaderClass->setVec3("viewPosition", viewportCamera.Position);
            shaderClass->setVec3("objectColor", objColor);
            shaderClass->setVec3("lightColor", lightColor);
            shaderClass->setVec3("lightPosition", lightPos);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 어떤 셰이더 프로그램을 사용할지 결정, 셰이더 프로그램의 id를 사용한다.
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glfw: 버퍼를 교체해서 다음 프레임으로 넘어가 주고, pollevent를 통해서 입출력을 받아준다.
        // 입출력은 키보드나 마우스 입력을 받는다.
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderClass->clear();
    lightShaderClass->clear();
    delete (shaderClass);
    delete (lightShaderClass);

    // glfw: 만약 glfw가 꺼지면, 해당 glfw에 대한 데이터를 완전히 없애준다.
    // 댕글링 방지용인듯
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void calcDeltaTime()
{
    float time = glfwGetTime();
    deltaTime = time - lastFrame;
    lastFrame = time;
}

// process all input: 해당 프레임에서 glfw에 관련된 키가 입력됐는지 확인하고, 쿼리한다.
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime, FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime,BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        viewportCamera.cameraMove(deltaTime,LEFT);
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

    //카메라가 뒤집어지지 않도록 값을 clamp
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