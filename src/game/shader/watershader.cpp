#include "game/shader/watershader.hpp"

WaterShader::WaterShader() {}

void WaterShader::init() 
{
    BaseShader::init("water_vertex.glsl", "water_fragment.glsl");
    getUniforms();
}

void WaterShader::getUniforms()
{
    bind();
    waterUniforms[PROJECTION_MATRIX] = glGetUniformLocation(program_id, "projectionMatrix");
    waterUniforms[MODELVIEW_MATRIX]  = glGetUniformLocation(program_id, "modelViewMatrix");
    waterUniforms[TEXTURE_SAMPLER]   = glGetUniformLocation(program_id, "my_sampler");
    // Light uniforms
    waterUniforms[AMBIENT_LIGHT]     = glGetUniformLocation(program_id, "ambientLight");  
    // Fog uniforms
    waterUniforms[FOG]               = glGetUniformLocation(program_id, "fog");
    waterUniforms[FOG_ACTIVE]        = glGetUniformLocation(program_id, "fog.active");
    waterUniforms[FOG_INTENSITY]     = glGetUniformLocation(program_id, "fog.intensity");
    waterUniforms[FOG_COLOR]         = glGetUniformLocation(program_id, "fog.color");
    waterUniforms[DELTA_T]           = glGetUniformLocation(program_id, "deltatime");
}

void WaterShader::setProjectionUniform(glm::mat4 pm)
{
    loadMat4(waterUniforms[PROJECTION_MATRIX], pm);
}

void WaterShader::setModelViewUniform(glm::mat4 vm)
{
    loadMat4(waterUniforms[MODELVIEW_MATRIX], vm);
}

void WaterShader::setTextureUniform(GLuint texture)
{
    loadFloat(waterUniforms[TEXTURE_SAMPLER], texture);
}

void WaterShader::setFogUniform(Fog fog)
{
    loadInt(waterUniforms[FOG_ACTIVE], (fog.active == 0) ? 0 : 1);
    loadFloat(waterUniforms[FOG_INTENSITY], fog.intensity);
    loadVec3(waterUniforms[FOG_COLOR], fog.color);
}

void WaterShader::setAmbientLightUniform(glm::vec3 ambient)
{
    loadVec3(waterUniforms[AMBIENT_LIGHT], ambient);
}

void WaterShader::bindTexture(GLuint idx)
{
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, idx);
}

void WaterShader::setDeltaTUniform(float dt)
{
    loadFloat(waterUniforms[DELTA_T], dt);
}