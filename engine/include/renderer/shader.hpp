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
protected:
    unsigned int _id;
    std::string _name;
};

class ShaderLoader{
public:
    // This is the addShader function for when I have a pre-built shader and just want to give it to the shaderloader's cache
    void addShader(const std::string& name, const std::shared_ptr<Shader>& shader );

    // This is the addShader function for when I want to buiild the shader from scratch and then push it to the cache
    void addShader(const std::string& name, const std::string& v_path, const std::string& f_path);

    // This function gets the shader if it exists in the cache, otherwise it returns nullptr
    std::shared_ptr<Shader> getShader(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> _shader_cache;
};
