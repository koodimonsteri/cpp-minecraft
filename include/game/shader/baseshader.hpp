#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

/*
 * Base shader class for all shaders.
 * Holds functions to set uniform values.
 */

class BaseShader
{
    public:
        BaseShader();
        virtual ~BaseShader();

        void init(const std::string& vertFile, const std::string& fragFile);

        void bind();
        void unbind();

        void loadInt(GLuint location, int val);
        void loadFloat(GLuint location, float val);
        void loadVec2(GLuint location, const glm::vec2 vec);
        void loadVec3(GLuint location, const glm::vec3 vec);
        void loadVec4(GLuint location, const glm::vec4 vec);
        void loadMat4(GLuint location, const glm::mat4 mat);
        
    protected:
        virtual void getUniforms() = 0;
        GLuint program_id;
};

namespace MyShaderUtil {

    void        checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    std::string loadShader(const std::string& filename);
    GLuint      createShader(const std::string& text, GLenum shaderType);

}