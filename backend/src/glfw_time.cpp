#include "core/delta_time.hpp"
#include <GLFW/glfw3.h>

float Time::_deltaTime = 0.0F;
float Time::_totalTime = 0.0F;
float Time::_current = 0.0F;
float Time::_lastFrame = 0.0F;

float Time::deltaTime() { return _deltaTime; }
float Time::totalTime() { return _totalTime; }

void Time::calculateDeltaTime(){
    _current = (float)glfwGetTime();
    _deltaTime = _current - _lastFrame;
    _lastFrame = _current;
}
