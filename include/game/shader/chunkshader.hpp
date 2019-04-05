#pragma once

#include "game/shader/baseshader.hpp"
#include "world/fog.hpp"

class ChunkShader : public BaseShader
{
    public:
        ChunkShader();

        void init();

        void setProjectionUniform(glm::mat4 pvm);
        void setModelViewUniform(glm::mat4 mm);
        void setTextureUniform(GLuint texture);
        void setAmbientLightUniform(glm::vec3 al);
        void setFogUniform(Fog fog);
        void bindTexture();
        void bindTexture(GLuint tex);
        void getUniforms() override;
    //private:
        enum {
            PROJECTION_MATRIX,
            MODELVIEW_MATRIX,
            AMBIENT_LIGHT,
            TEXTURE_SAMPLER,
            FOG, FOG_ACTIVE, FOG_INTENSITY, FOG_COLOR,
            NUM_UNIFORMS
        };
        GLuint chunkUniforms[NUM_UNIFORMS];

        GLuint texture_idx;
};