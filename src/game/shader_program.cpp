#include "game/shader_program.hpp"

// STL
#include <fstream>
#include <iostream>
#include <functional>

// Own
#include "game/event/event_util.hpp"

ShaderProgram::ShaderProgram() {}

ShaderProgram::~ShaderProgram() {
    for (auto shader : shaders_) {
        glDetachShader(program_, shader);
        glDeleteShader(shader);
    }
    glDeleteProgram(program_);
}

void ShaderProgram::init() {
            // Event callbacks
    using namespace std::placeholders;
    EventHandler<glm::mat4>::bindAction(EventType::UPDATE_VIEW_PROJECTION, std::bind(&ShaderProgram::setViewProjectionUniform, this, _1));
    EventHandler<glm::mat4>::bindAction(EventType::UPDATE_MODEL, std::bind(&ShaderProgram::setModelUniform, this, _1));
    EventHandler<GLuint>::bindAction(EventType::UPDATE_TEXTURE, std::bind(&ShaderProgram::setTextureUniform, this, _1));

    using namespace ShaderUtil;
    std::string shaderDir = std::string("../resource/shader/");
    program_ = glCreateProgram();
    shaders_[VERTEX_SHADER] = createShader(loadShader(shaderDir + "chunk_vertex.glsl"), GL_VERTEX_SHADER);
    shaders_[FRAGMENT_SHADER] = createShader(loadShader(shaderDir + "chunk_fragment.glsl"), GL_FRAGMENT_SHADER);

    glAttachShader(program_, shaders_[VERTEX_SHADER]);
    glAttachShader(program_, shaders_[FRAGMENT_SHADER]);

    glLinkProgram(program_);
    checkShaderError(program_, GL_LINK_STATUS, true, "Error: Program failed to link: ");
    glValidateProgram(program_);
    checkShaderError(program_, GL_LINK_STATUS, true, "Error: Program failed to validate: ");
    uniforms_[VIEWPROJECTION_U]   = glGetUniformLocation(program_, "viewProjectionMatrix");
    uniforms_[MODEL_U]            = glGetUniformLocation(program_, "modelMatrix");
    uniforms_[TEXTURE_U]  = glGetUniformLocation(program_, "my_sampler");
}

void ShaderProgram::bind() {
    glUseProgram(program_);
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

void ShaderProgram::setTextureUniform(GLuint texture) const {
    glUniform1i(uniforms_[TEXTURE_U], texture);
}

void ShaderProgram::setViewProjectionUniform(glm::mat4 viewProjection) const {
    glUniformMatrix4fv(uniforms_[VIEWPROJECTION_U], 1, GL_FALSE, &viewProjection[0][0]);
}

void ShaderProgram::setModelUniform(glm::mat4 model) const {
    glUniformMatrix4fv(uniforms_[MODEL_U], 1, GL_FALSE, &model[0][0]);
}

std::string ShaderUtil::loadShader(const std::string& filename) { 
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

void ShaderUtil::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
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

GLuint ShaderUtil::createShader(const std::string& text, GLenum shaderType) {
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