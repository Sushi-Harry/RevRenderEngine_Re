#pragma once

#include <algorithm>

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

enum class camera_movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;

class Camera3D{
public:
    Camera3D(const glm::vec3& pos = {0.0, 0.0, 0.0}, const glm::vec3& up = {0.0, 1.0, 0.0}, float yaw = YAW, float pitch = PITCH) :
        _front({0.0, 0.0, -1.0}),
        _mv_speed(SPEED),
        _mouse_sensitivity(SENSITIVITY)
    {
        _position = pos;
        _world_up = up;
        _yaw = yaw;
        _pitch = pitch;
        _projection = glm::perspective(glm::radians(45.0F), 16.0F / 9.0F, 0.1F, 100.0F);
        update_cam_vectors();
    }

    Camera3D(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _mv_speed(SPEED), _mouse_sensitivity(SENSITIVITY){
        _position = glm::vec3(posX, posY, posZ);
        _world_up = glm::vec3(upX, upY, upZ);
        _yaw = yaw;
        _pitch = pitch;
        update_cam_vectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 getProjectionMatrix() const {
        return _projection;
    }

    glm::mat4 getViewProjMatrix() const {
        return _projection * getViewMatrix();
    }

    glm::vec3 getViewPos() const {
        return _position;
    }

    void processMouseMovement(float x_offset, float y_offset){
        x_offset *= _mouse_sensitivity;
        y_offset *= _mouse_sensitivity;

        _yaw += x_offset;
        _pitch += y_offset;

        _pitch = std::min(_pitch, 89.0f);
        _pitch = std::max(_pitch, -89.0f);

        update_cam_vectors();
    }

    void processKeyboard(camera_movement dir, float deltaTime){
        float velocity = _mv_speed * deltaTime;
        switch (dir) {

            case camera_movement::FORWARD:
                _position += _front * velocity;
                break;
            case camera_movement::BACKWARD:
                _position -= _front * velocity;
                break;
            case camera_movement::LEFT:
                _position -= _right * velocity;
                break;
            case camera_movement::RIGHT:
                _position += _right * velocity;
                break;
        }
    }

    void setProjection(glm::mat4 proj) { _projection = proj; }
private:
    void update_cam_vectors(){
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        _right = glm::normalize(glm::cross(_front, _world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        _up = glm::normalize(glm::cross(_right, _front));
    }

    glm::mat4 _projection;
    glm::vec3 _position = {0.0f, 0.0f, 0.0f};
    glm::vec3 _up, _right, _front, _world_up;

    float _yaw, _pitch, _mv_speed, _mouse_sensitivity;
};
