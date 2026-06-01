#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <memory>

#include "glm/glm.hpp"

#include <unordered_map>

class Shader{
public:
    virtual ~Shader() = default;

    virtual void bindShader() = 0;
    virtual void unbindShader() = 0;

    // Functions for setting
    virtual void setInt(const std::string& name, int val) const = 0;
    virtual void setBool(const std::string& name, bool value) const = 0;
    virtual void setFloat(const std::string& name, float value) const = 0;
    virtual void setVec2(const std::string& name, const glm::vec2& vec) const = 0;
    virtual void setVec3(const std::string& name, const glm::vec3& vec) const = 0;
    virtual void setVec4(const std::string& name, const glm::vec4& vec) const = 0;
    virtual void setMat2(const std::string& name, const glm::mat2& mat) const = 0;
    virtual void setMat3(const std::string& name, const glm::mat3& mat) const = 0;
    virtual void setMat4(const std::string& name, const glm::mat4& mat) const = 0;

    virtual std::string getName() const = 0;

    static std::shared_ptr<Shader> Create(const std::string& name, const std::string& v_path, const std::string& f_path);

    uint32_t _id;
protected:
    std::string _name;
};
