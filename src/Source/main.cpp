#include <glad/glad.h>
#include <glfw/3.4/include/GLFW/glfw3.h>
#include <iostream>
#include "public/FileSupport.h"
#include "public/Shader.h"

// 프레임 버퍼 함수 크기 및 프로세스 입력 함수 선언.
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// 기본 해상도 설정.
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    std::cout << "GLFW context version: " << currentMajorVersion << "." << currentMinorVersion << std::endl;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 초기화.
    // glad에 현재 glfw의 프로세스 주소를 넘겨서 현재 glad가 glfw를 관리할수 있도록 해 준다.
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shader코드를 가져와서 컴파일 이후 프로그램 id를 할당.
    std::string vertexShaderCodeStr;
    FileFromString("src/ClassFolder/private/Shader.vs", vertexShaderCodeStr);

    std::string fragmentShaderCodeStr;
    FileFromString("src/ClassFolder/private/Shader.fs", fragmentShaderCodeStr);

    Shader *shaderClass = new Shader(vertexShaderCodeStr.c_str(), fragmentShaderCodeStr.c_str());
    shaderClass->CreateShader(vertexShaderCodeStr, fragmentShaderCodeStr);

    float vertices[] = {
        // 위치              // 컬러
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 우측 하단
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 좌측 하단
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 위
    };
    unsigned int indices[] = {
        0, 1, 2};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop.
    // 현재 glfw window의 메인 루프를 진행한다.
    // 현재 윈도우가 켜져 있으면, 입력을 받고, 버퍼를 교체하고, 이벤트를 돌려준다.
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // shader의 사용 설정을 해 준다.
        shaderClass->use();

        // uniform을 사용해서 색상을 변경해 준다.
        //  float elapseTime = glfwGetTime();
        //  float sinColorValue = sin(elapseTime) / 2.0f + 0.5f;
        //  int vertexshadervalue = glGetUniformLocation(shaderProgramID, "ourColor");
        //  glUniform4f(vertexshadervalue, sinColorValue, 0.0f,0.0f,1.0f);

        // 어떤 셰이더 프로그램을 사용할지 결정, 셰이더 프로그램의 id를 사용한다.
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glfw: 버퍼를 교체해서 다음 프레임으로 넘어가 주고, pollevent를 통해서 입출력을 받아준다.
        // 입출력은 키보드나 마우스 입력을 받는다.
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderClass->clear();
    delete (shaderClass);

    // glfw: 만약 glfw가 꺼지면, 해당 glfw에 대한 데이터를 완전히 없애준다.
    // 댕글링 방지용인듯
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: 해당 프레임에서 glfw에 관련된 키가 입력됐는지 확인하고, 쿼리한다.
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 윈도우 크기 변경 시 해당 함수 호출.
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // 뷰포트와 새창 크기의 일치를 확인.
    glViewport(0, 0, width, height);
}