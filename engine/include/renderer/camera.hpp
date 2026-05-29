#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class OrthoCam{
public:
    OrthoCam(float left, float right, float bottom, float top);

    const glm::vec3& getPosition() const { return _position; }
    float getRotation() const { return _rotation; }
    const glm::mat4& getProjection() const { return _projection; }
    const glm::mat4& getView() const { return _view; }
    const glm::mat4& getViewProj() const { return _view_proj; }

    void setPosition(const glm::vec3& pos) { _position = pos; recalculateView(); }
    void setRotation(float rot) { _rotation = rot; recalculateView(); }
    void setProjection(float left, float right, float bottom, float top);

private:
    void recalculateView();

    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _view_proj;
    glm::vec3 _position = {0.0f, 0.0f, 0.0f};
    float _rotation = 0.0f;
};
