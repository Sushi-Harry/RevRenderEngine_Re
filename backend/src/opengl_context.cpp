#include "opengl_context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : _windowHandle(windowHandle){
    if(!_windowHandle){
        std::cout << "ERROR::OPENGL_CONTEXT::NULL_WINDOW_HANDLE\n";
    }
}

void OpenGLContext::Init(){
    glfwMakeContextCurrent(_windowHandle);

    // GLAD initialization code
    if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0){
        std::cout << "ERROR::OPENGL_CONTEXT::GLAD_INIT_FAILURE\n";
        return;
    }
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(_windowHandle);
}
