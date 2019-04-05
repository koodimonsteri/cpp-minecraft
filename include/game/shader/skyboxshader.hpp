#pragma once

#include "game/shader/baseshader.hpp"

class SkyBoxShader : public BaseShader
{
    public:
        SkyBoxShader();

        void init();

        void setProjectionUniform(glm::mat4 pm);
        void setViewUniform(glm::mat4 vm);
        void setTextureUniform(GLuint texture);
        void setAmbientLightUniform(glm::vec3 al);
        void bindTexture();
        void bindTexture(GLuint idx);

        void getUniforms() override;

        enum {
            PROJECTION_MATRIX,
            VIEW_MATRIX,
            AMBIENT_LIGHT,
            TEXTURE_SAMPLER,
            NUM_UNIFORMS
        };
        GLuint skyboxUniforms[NUM_UNIFORMS];
     
        GLuint texture_idx;
};