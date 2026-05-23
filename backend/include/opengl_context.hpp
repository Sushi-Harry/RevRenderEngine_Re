#pragma once
#include "renderer/graphics_context.hpp"

// using a forward declaration here so I don't have to include glfw3.h in this file
struct GLFWwindow;
class OpenGLContext : public GraphicsContext{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* _windowHandle;
};
