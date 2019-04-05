#pragma once

#include "game/shader/baseshader.hpp"
#include "world/fog.hpp"

class WaterShader : public BaseShader
{
    public:
        WaterShader();

        void init();

        void setProjectionUniform(glm::mat4 pm);
        void setModelViewUniform(glm::mat4 vm);
        void setTextureUniform(GLuint texture);
        void setAmbientLightUniform(glm::vec3 ambient);
        void setFogUniform(Fog fog);
        void setDeltaTUniform(float dt);
        void bindTexture(GLuint idx);

        void getUniforms();

        enum {
            PROJECTION_MATRIX,
            MODELVIEW_MATRIX,
            AMBIENT_LIGHT,
            TEXTURE_SAMPLER,
            FOG, FOG_ACTIVE, FOG_INTENSITY, FOG_COLOR,
            DELTA_T,
            NUM_UNIFORMS 
        };
        GLuint waterUniforms[NUM_UNIFORMS];
};