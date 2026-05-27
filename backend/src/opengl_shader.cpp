#include "opengl_shader.hpp"

std::shared_ptr<Shader> Shader::Create(const std::string &name, const std::string &v_path, const std::string &f_path){
    return std::make_shared<opengl_shader>(name, v_path, f_path);
}

opengl_shader::opengl_shader(const std::string &name, const std::string &v_path, const std::string &f_path) {
    _name = name;

    std::string vert_code, frag_code;

    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        vShaderFile.open(v_path); fShaderFile.open(f_path);

        std::stringstream vStream, fStream;

        vStream << vShaderFile.rdbuf(); fStream << fShaderFile.rdbuf();

        vShaderFile.close(); fShaderFile.close();

        vert_code = vStream.str(); frag_code = fStream.str();
    }catch(std::ifstream::failure& e){
        std::cout << "ERROR::SHADER_LOADING::CANNOT_READ_FILE\n";
    }
    // Need the const char* type stuff later
    const char *cc_vert = vert_code.c_str();
    const char *cc_frag = frag_code.c_str();

    unsigned int vert, frag;

    // Creating the shaders
    vert = glCreateShader(GL_VERTEX_SHADER); frag = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert, 1, &cc_vert, nullptr);
    glShaderSource(frag, 1, &cc_frag, nullptr);

    glCompileShader(vert);
    glCompileShader(frag);

    opengl_shader::checkCompilationErrors(vert, "VERTEX");
    opengl_shader::checkCompilationErrors(frag, "FRAGMENT");

    _id = glCreateProgram();
    glAttachShader(_id, vert);
    glAttachShader(_id, frag);
    glLinkProgram(_id);

    opengl_shader::checkCompilationErrors(_id, "PROGRAM");
    glDeleteShader(vert);
    glDeleteShader(frag);
}

opengl_shader::~opengl_shader(){
    glDeleteProgram(_id);
}

// THis is copied from my previous attempt at the rewrite. Though I'm sure I'm still not the author of this function
void opengl_shader::checkCompilationErrors(unsigned int shader, const std::string &type){
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(success == 0)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(success == 0)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void opengl_shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}
void opengl_shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}
void opengl_shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}
void opengl_shader::setVec2(const std::string& name, const glm::vec2& vec) const {
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &vec[0]);
}
void opengl_shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &vec[0]);
}
void opengl_shader::setVec4(const std::string& name, const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &vec[0]);
}
void opengl_shader::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void opengl_shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void opengl_shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string opengl_shader::getName() const { return _name; }

void opengl_shader::bindShader() {
    glUseProgram(_id);
}
void opengl_shader::unbindShader(){
    glUseProgram(0);
}
