#include "game/shader/chunkshader.hpp"
#include "util/png_image.hpp"
#include "util/texture_atlas.hpp"

ChunkShader::ChunkShader() {}

void ChunkShader::init()
{
    BaseShader::init("chunk_vertex.glsl", "chunk_fragment.glsl");

    getUniforms();

   /* PngImage image = PngImage("../resource/texture/minecraft_atlas.png");
    TextureAtlas atlas = TextureAtlas(image, 16, 16, 0);
    atlas.init(texture_idx);*/

    //setTextureUniform(0);
}

void ChunkShader::getUniforms()
{
    bind();
    chunkUniforms[PROJECTION_MATRIX] = glGetUniformLocation(program_id, "projectionMatrix");
    chunkUniforms[MODELVIEW_MATRIX]  = glGetUniformLocation(program_id, "modelViewMatrix");
    chunkUniforms[TEXTURE_SAMPLER]   = glGetUniformLocation(program_id, "my_sampler");
    chunkUniforms[AMBIENT_LIGHT]     = glGetUniformLocation(program_id, "ambientLight");  
    chunkUniforms[FOG]               = glGetUniformLocation(program_id, "fog");
    chunkUniforms[FOG_ACTIVE]        = glGetUniformLocation(program_id, "fog.active");
    chunkUniforms[FOG_INTENSITY]     = glGetUniformLocation(program_id, "fog.intensity");
    chunkUniforms[FOG_COLOR]         = glGetUniformLocation(program_id, "fog.color");
}

void ChunkShader::setProjectionUniform(glm::mat4 pm)
{
    loadMat4(chunkUniforms[PROJECTION_MATRIX], pm);
}

void ChunkShader::setModelViewUniform(glm::mat4 mvm)
{
    loadMat4(chunkUniforms[MODELVIEW_MATRIX], mvm);
}

void ChunkShader::setTextureUniform(GLuint texture) 
{
    loadInt(chunkUniforms[TEXTURE_SAMPLER], texture);
}

void ChunkShader::setAmbientLightUniform(glm::vec3 al)
{
    loadVec3(chunkUniforms[AMBIENT_LIGHT], al);
}

void ChunkShader::setFogUniform(Fog fog)
{
    loadInt(chunkUniforms[FOG_ACTIVE], (fog.active == 0) ? 0 : 1);
    loadFloat(chunkUniforms[FOG_INTENSITY], fog.intensity);
    loadVec3(chunkUniforms[FOG_COLOR], fog.color);
}

void ChunkShader::bindTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_idx);
}

void ChunkShader::bindTexture(GLuint idx)
{
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, idx);
}