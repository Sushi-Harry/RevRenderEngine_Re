#include "glfw_input.hpp"

Input* Input::_instance = new glfw_input();

bool glfw_input::v_isKeyPressed(Key keycode) {
    auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
    auto key_state = glfwGetKey(window, static_cast<int>(keycode));
    return key_state == GLFW_PRESS || key_state == GLFW_REPEAT;
}

bool glfw_input::v_isMBPressed(Mouse mbcode) {
    auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
    auto mb_state = glfwGetMouseButton(window, static_cast<int>(mbcode));
    return mb_state == GLFW_PRESS || mb_state == GLFW_REPEAT;
}

double glfw_input::v_getX() {
    auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
    double x_pos = -1.0;
    glfwGetCursorPos(window, &x_pos, nullptr);
    return x_pos;
}

double glfw_input::v_getY() {
    auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
    double y_pos = -1.0;
    glfwGetCursorPos(window, nullptr, &y_pos);
    return y_pos;
}

glm::vec2 glfw_input::v_getMousePos(){
    auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
    double x_pos = -1.0;
    double y_pos = -1.0;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    return {x_pos, y_pos};
}
