#include "game/shader/skyboxshader.hpp"
#include "util/png_image.hpp"
#include "util/texture_atlas.hpp"

SkyBoxShader::SkyBoxShader() {}

void SkyBoxShader::init()
{
    BaseShader::init("skybox_vertex.glsl", "skybox_fragment.glsl");
    getUniforms();

    /*PngImage image = PngImage("../resource/texture/skybox.png");
    TextureAtlas atlas = TextureAtlas(image, 2, 3, 1);
    atlas.init(texture_idx);*/

    setTextureUniform(1);
}

void SkyBoxShader::getUniforms()
{
    bind();
    skyboxUniforms[PROJECTION_MATRIX] = glGetUniformLocation(program_id, "projectionMatrix");
    skyboxUniforms[VIEW_MATRIX]       = glGetUniformLocation(program_id, "viewMatrix");
    skyboxUniforms[TEXTURE_SAMPLER]   = glGetUniformLocation(program_id, "sb_sampler");
    skyboxUniforms[AMBIENT_LIGHT]     = glGetUniformLocation(program_id, "ambientLight");    
}

void SkyBoxShader::setProjectionUniform(glm::mat4 pm)
{
    loadMat4(skyboxUniforms[PROJECTION_MATRIX], pm);
}

void SkyBoxShader::setViewUniform(glm::mat4 vm)
{
    loadMat4(skyboxUniforms[VIEW_MATRIX], vm);
}

void SkyBoxShader::setAmbientLightUniform(glm::vec3 al)
{
    loadVec3(skyboxUniforms[AMBIENT_LIGHT], al);
}

void SkyBoxShader::setTextureUniform(GLuint texture)
{
    glUniform1i(skyboxUniforms[TEXTURE_SAMPLER], texture);
}

void SkyBoxShader::bindTexture()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_idx);
}

void SkyBoxShader::bindTexture(GLuint idx)
{
    //glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, idx);
}