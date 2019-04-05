#pragma once

// STL
#include <string>

// 3rd party
#include "glad/glad.h"
#include "glm/glm.hpp"

class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        void init();
        void bind();
        void unbind();
        void setTextureUniform(GLuint texture) const;
        void setViewProjectionUniform(glm::mat4 viewProjection) const;
        void setModelUniform(glm::mat4 model) const;
    
    private:
        enum { VIEWPROJECTION_U, MODEL_U, TEXTURE_U, NUM_UNIFORMS };
        enum { VERTEX_SHADER, FRAGMENT_SHADER, NUM_SHADERS };
        GLuint program_;
        GLuint shaders_[NUM_SHADERS];
        GLuint uniforms_[NUM_UNIFORMS];
};

namespace ShaderUtil {
    void        checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    std::string loadShader(const std::string& filename);
    GLuint      createShader(const std::string& text, GLenum shaderType);
}