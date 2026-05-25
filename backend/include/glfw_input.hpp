#pragma once
#include "core/application.hpp"
#include "core/input.hpp"
#include <GLFW/glfw3.h>

class glfw_input : public Input {
public:
    // This function communicates with glfw to check if the Key with the given keycode has been pressed or not
    bool v_isKeyPressed(Key keycode) override;
    bool v_isMBPressed(Mouse mbcode) override;
    double v_getX() override;
    double v_getY() override;
    glm::vec2 v_getMousePos() override;
private:

};
