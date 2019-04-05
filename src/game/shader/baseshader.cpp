#include "game/shader/baseshader.hpp"

#include <fstream>
#include <iostream>
#include <functional>

BaseShader::BaseShader()
{
}

BaseShader::~BaseShader()
{
    glDeleteProgram(program_id);
}

void BaseShader::init(const std::string& vertFile, const std::string& fragFile)
{
    std::string shaderDir = std::string("../resource/shader/");

    program_id = glCreateProgram();

    using namespace MyShaderUtil;
    auto vertex_shader   = createShader(loadShader(shaderDir + vertFile), GL_VERTEX_SHADER);
    auto fragment_shader = createShader(loadShader(shaderDir + fragFile), GL_FRAGMENT_SHADER);

    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);

    glLinkProgram(program_id);
    checkShaderError(program_id, GL_LINK_STATUS, true, "Error: Program failed to link: ");

    glValidateProgram(program_id);
    checkShaderError(program_id, GL_LINK_STATUS, true, "Error: Program failed to validate: ");

    glDetachShader(program_id, vertex_shader);
    glDetachShader(program_id, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void BaseShader::bind()
{
    glUseProgram(program_id);
}

void BaseShader::unbind()
{
    glUseProgram(0);
}

void BaseShader::loadInt(GLuint location, int val)
{
    glUniform1i(location, val);
}

void BaseShader::loadFloat(GLuint location, float val)
{
    glUniform1f(location, val);
}

void BaseShader::loadVec2(GLuint location, const glm::vec2 vec)
{
    glUniform2f(location, vec.x, vec.y);
}

void BaseShader::loadVec3(GLuint location, const glm::vec3 vec)
{
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void BaseShader::loadVec4(GLuint location, const glm::vec4 vec)
{
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void BaseShader::loadMat4(GLuint location, const glm::mat4 mat)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

// ShaderUtil functions to load and compile shaders

std::string MyShaderUtil::loadShader(const std::string& filename) { 
    std::ifstream file;
    file.open((filename).c_str());
    std::string output;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << filename << std::endl;
    }
    return output;
}

void MyShaderUtil::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
    GLint  success     = 0;
    GLchar error[1024] = {0};
    if (isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);
    if (success == GL_FALSE) {
        if (isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint MyShaderUtil::createShader(const std::string& text, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) std::cerr << "Error: Shader creation failed!" << std::endl;
    const GLchar* shaderSourceStrings[1];
    GLint         shaderSourceStringLenghts[1];
    shaderSourceStrings[0]       = text.c_str();
    shaderSourceStringLenghts[0] = text.length();
    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLenghts);
    glCompileShader(shader);
    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");
    return shader;
}