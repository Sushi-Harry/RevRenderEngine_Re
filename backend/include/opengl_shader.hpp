#pragma once

#include "glad/glad.h"
#include "renderer/shader.hpp"

class opengl_shader : public Shader{
public:
    opengl_shader(const std::string& name, const std::string& v_path, const std::string& f_path);
    ~opengl_shader();

    void bindShader() override;
    void unbindShader() override;

    void setInt(const std::string& name, int val) const override;
    void setBool(const std::string& name, bool value) const override;
    void setFloat(const std::string& name, float value) const override;
    void setVec2(const std::string& name, const glm::vec2& vec) const override;
    void setVec3(const std::string& name, const glm::vec3& vec) const override;
    void setVec4(const std::string& name, const glm::vec4& vec) const override;
    void setMat2(const std::string& name, const glm::mat2& mat) const override;
    void setMat3(const std::string& name, const glm::mat3& mat) const override;
    void setMat4(const std::string& name, const glm::mat4& mat) const override;

    std::string getName() const override;

    static void checkCompilationErrors(unsigned int shader, const std::string& type);
};
