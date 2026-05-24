#pragma once

#include <glm/glm.hpp>
#include "core/keys.hpp"
#include "core/mouse.hpp"

class Input {
public:
    virtual bool isKeyPressed(Key keycode);
    static bool isMBPressed(Mouse mbcode);
    static glm::vec2 getMousePos();
    static float getX();
    static float getY();
};
