#include <GLFW/glfw3.h>

//glfw를 사용해서 opengl을 사용하는 main 함수.
int main()
{
    //opengl을 윈도우로 사용하기 위한 기본 변수.
    GLFWwindow* window;
    if(!glfwInit())
    {
        return -1;
    }

    //해당 윈도우에 윈도우를 생성함, 이때 해상도 및 윈도우 이름을 설정.
    //만약 해당 사항에서, 윈도우 생성이 되지 않으면, glfw윈도우를 제거.
    window = glfwCreateWindow(800, 400, "Hello World", NULL,NULL);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    //윈도우 생성이 됐다면, 현재 윈도우를 설정.
    glfwMakeContextCurrent(window);

    //윈도우가 닫히지 않는다면, gl 클리어 -> gl버퍼 변경 -> gl의 다중 입출력 이벤트를 실행
    //해당 순서로 메인 함수를 진행한다.
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //만약 닫혔다면 생성실패와 동일하게 terminate 진행.
    glfwTerminate();
    return 0;
}