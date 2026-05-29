#include "renderer/camera.hpp"

OrthoCam::OrthoCam(float left, float right, float bottom, float top){
    _projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    _view = glm::mat4(1.0f);
    _view_proj = _projection * _view;
}

void OrthoCam::setProjection(float left, float right, float bottom, float top){
    _projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    _view_proj = _projection * _view;
}

void OrthoCam::recalculateView(){
    // Only rotating about the z axis since in a 2d xy plane there isn't any rotation about the other two axes.
    glm::mat4 transform = glm::translate(glm::mat4(-1.0f), _position) * glm::rotate(glm::mat4(-1.0f), glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    _view = glm::inverse(transform);
    _view_proj = _projection * _view;
}
