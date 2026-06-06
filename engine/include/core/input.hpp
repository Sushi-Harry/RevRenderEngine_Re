#pragma once

#include <glm/glm.hpp>
#include "core/keys.hpp"
#include "core/mouse.hpp"
#include "core/window.hpp"

//   \      /
//    \    /
//     \  /
//      \/ _______     Means that the function is supposed to be virtual and spposed to be overridden in the backend class

class Input {
public:
    static void Init();
    inline static bool isKeyPressed(Key keycode) { return _instance->v_isKeyPressed(keycode); }
    inline static bool isMBPressed(Mouse mbcode) { return _instance->v_isMBPressed(mbcode); }
    inline static glm::vec2 getMousePos() { return _instance->v_getMousePos(); }
    inline static double getX() { return _instance->v_getX(); }
    inline static double getY() { return _instance->v_getY(); }

protected:
    virtual bool v_isKeyPressed(Key keycode) = 0;
    virtual bool v_isMBPressed(Mouse mbcode) = 0;
    virtual glm::vec2 v_getMousePos() = 0;
    virtual double v_getX() = 0;
    virtual double v_getY() = 0;

private:
    static Input* _instance;
protected:
};
