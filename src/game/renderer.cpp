#include "game/renderer.hpp"
// STL
#include <iostream>
#include <vector>
#include <functional>

// 3rd party
#include "glad/glad.h"

// Own
#include "game/event/event_util.hpp"
#include "util/png_image.hpp"
#include "util/texture_atlas.hpp"
#include "world/block.hpp"
#include "world/chunk.hpp"
#include "world/world.hpp"
#include "util/timer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "util/math_constants.hpp"

Renderer::Renderer() {}

void Renderer::init() {
    // Event callbacks
    using namespace std::placeholders;
    EventHandler<const void*, Renderable>::bindAction(EventType::UPDATE_RENDERABLE, std::bind(&Renderer::updateRenderable, this, _1, _2));
    EventHandler<const void*>::bindAction(EventType::DELETE_RENDERABLE, std::bind(&Renderer::deleteRenderable, this, _1));
    EventHandler<glm::mat4>::bindAction(EventType::UPDATE_VIEW_MATRIX, std::bind(&Renderer::updateViewMatrix, this, _1));

    EventHandler<const void*, RenderableContainer>::bindAction(EventType::UPDATE_CHUNK_RENDERABLE, std::bind(&Renderer::updateChunkRenderables, this, _1, _2));
    EventHandler<const void*>::bindAction(EventType::DELETE_CHUNK_RENDERABLE, std::bind(&Renderer::clearChunkRenderables, this, _1));

    projectionMatrix = glm::perspective(1.5f, 8.0f/6.0f, 0.01f, 1000.0f);

    // For now textures are initialized here.
    // In the future when blocks are not the only thing having textures, a more
    // complex solution will be implemented.
    initTextures();

    chunkShader_.init();
    skyboxShader_.init();
    waterShader_.init();
}

void Renderer::updateRenderable(const void* key, const Renderable& renderable) {
    if (renderables_.find(key) != renderables_.end()) {
        glDeleteVertexArrays(1, &renderables_[key].vao);
        renderables_[key].vao = renderable.vao;
        renderables_[key].size = renderable.size;
    } else {
        renderables_[key] = renderable;
    }
}

void Renderer::updateChunkRenderables(const void* key, const RenderableContainer& chunkrenderable)
{
    // Check if chunkRenderable is already in renderer
    // If it is, loop through renderables and update them to renderer (compare renderable type and vao)
    // Else add new entry to renderables
    auto it = chunkRenderables.find(key);
    if(it != chunkRenderables.end())    // If RenderableContainer is already in renderer
    {
        //LOG_CRITICAL("UPDATED EXISTING CHUNK RENDERABLE");
        for(auto newRenderable : chunkrenderable.renderables)   // New renderables
        {
            bool found = false;
            for(auto oldRenderable : it->second.renderables)  // Old renderables
            {
                if(newRenderable.type == oldRenderable.type && newRenderable.vao != oldRenderable.vao)    // If renderables are same type and vaos are different
                {
                    found = true;
                    glDeleteVertexArrays(1, &oldRenderable.vao);
                    oldRenderable.vao = newRenderable.vao;
                    oldRenderable.size = newRenderable.size;
                }
            }
            if(!found){  // If mesh was not already in chunkRenderable
                it->second.renderables.push_back(newRenderable);
            }
        }
    } else {   // If RenderableContainer is not in renderer
        //LOG_CRITICAL("ADDED NEW CHUNK RENDERABLE");
        chunkRenderables[key] = chunkrenderable;
        //LOG_CRITICAL("CHUNKRENDERABLESIZE", chunkrenderable.renderables.size(), "RENDERABLES2SIZE", chunkRenderables[key].renderables.size());
    }
}

// Updates only 1 renderable in chunk
void Renderer::updateChunkRenderable(const void* key, const Renderable& renderable)
{
    auto it = chunkRenderables.find(key);
    if(it != chunkRenderables.end())
    {
        bool found = false;
        for(auto oldRenderable : it->second.renderables)
        {
            if(oldRenderable.type == renderable.type && oldRenderable.vao != renderable.vao)
            {
                glDeleteVertexArrays(1, &oldRenderable.vao);
                oldRenderable.vao = renderable.vao;
                oldRenderable.size = renderable.size;
                found = true;
            }
        }
        if(!found)
        {
            it->second.renderables.push_back(renderable);
        }
    } else {
        RenderableContainer rc;
        rc.chunkBB = renderable.bb;
        rc.renderables.push_back(renderable);
        chunkRenderables[key] = rc;
    }
}

void Renderer::render(World* world, float dt, float waterdt)
{
    glClearColor(0.3, 0.3, 0.3, 0.1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto tempPlayer = world->getPlayer();
    glm::vec3 newPos = tempPlayer.getPosition() + (tempPlayer.getVelocity() * dt * (20.0f / 100.0f));
    
    viewMatrix = glm::lookAt(newPos, newPos + tempPlayer.getFacing(), UP);

    // Extract frustum planes from projectionViewMatrix
    frustumCulling_.getFrustumPlanes(projectionMatrix * viewMatrix);
    
    // render SkyBox, needs custom viewMatrix
    // render Chunks
    renderSkyBox(world);
    //renderChunks(world);
    renderChunkRenderables(world, waterdt);
}

void Renderer::renderChunks(World* world)
{    
    //Timer timer("Rendering chunks");
    chunkShader_.bind();
    chunkShader_.setProjectionUniform(projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    chunkShader_.bindTexture(textureAtlas);
    chunkShader_.setAmbientLightUniform(world->getAmbientLight());
    chunkShader_.setFogUniform(world->getFog());
    //LOG_TRACE("renderables_ size", renderables_.size());
    for (auto pair : renderables_) {
        Renderable renderable = pair.second;
        if(frustumCulling_.isInsideFrustum(renderable.bb))
        {
            chunkShader_.setModelViewUniform(viewMatrix * renderable.model);

            glBindVertexArray(renderable.vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderable.size);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glBindVertexArray(0);
        }
    }
    chunkShader_.unbind();
}

// Render function checks RenderableContainer boundingbox, if it is in view frustum, loop through renderables and push them to own vectors
// Then call own render functions for solid/unsolid meshes and pass those vectors as parameters.
void Renderer::renderChunkRenderables(World* world, float waterdt)
{
    std::vector<Renderable> solidRenderables, waterRenderables;
    for(auto chunkRenderable : chunkRenderables)
    {
        if(frustumCulling_.isInsideFrustum(chunkRenderable.second.chunkBB))
        {
            for(auto renderable : chunkRenderable.second.renderables)
            {
                switch(renderable.type)
                {
                    case RenderableType::SOLID   : solidRenderables.push_back(renderable); break;
                    case RenderableType::UNSOLID : waterRenderables.push_back(renderable); break;
                }
            }
        }
    }
    renderSolidMeshes(world, solidRenderables);
    renderWaterMeshes(world, waterRenderables, waterdt);
}

void Renderer::renderSolidMeshes(World* world, std::vector<Renderable> rend)
{
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    chunkShader_.bind();
    chunkShader_.setProjectionUniform(projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    chunkShader_.bindTexture(textureAtlas);
    chunkShader_.setAmbientLightUniform(world->getAmbientLight());
    chunkShader_.setFogUniform(world->getFog());

    for (auto renderable : rend) {
        chunkShader_.setModelViewUniform(viewMatrix * renderable.model);

        glBindVertexArray(renderable.vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderable.size);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }
    chunkShader_.unbind();
}

void Renderer::renderWaterMeshes(World* world, std::vector<Renderable> rend, float dt)
{
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    waterShader_.bind();
    waterShader_.setProjectionUniform(projectionMatrix);

    waterShader_.setAmbientLightUniform(world->getAmbientLight());
    waterShader_.setFogUniform(world->getFog());
    waterShader_.setDeltaTUniform(dt);

    glActiveTexture(GL_TEXTURE0);
    waterShader_.bindTexture(textureAtlas);

    for(Renderable renderable : rend)
    {
        waterShader_.setModelViewUniform(viewMatrix * renderable.model);

        glBindVertexArray(renderable.vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderable.size);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindVertexArray(0);
    }
    waterShader_.unbind();
}

void Renderer::renderSkyBox(World* world)
{
    //Timer timer("Rendering skybox");
    SkyBox* tempBox = world->getSkyBox();
    if(tempBox)
    {
        // Set viewMatrix position to 0
        glm::mat4 tempVM = viewMatrix;
        tempVM[3][0] = 0;
        tempVM[3][1] = 0;
        tempVM[3][2] = 0;

        skyboxShader_.bind();
        skyboxShader_.setProjectionUniform(projectionMatrix);
        skyboxShader_.setViewUniform(tempVM);
        skyboxShader_.setAmbientLightUniform(world->getAmbientLight());

        glActiveTexture(GL_TEXTURE1);
        skyboxShader_.bindTexture(skyboxTexture);
        
        Renderable tempR = tempBox->getRenderable();

        glBindVertexArray(tempR.vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, tempR.size);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);

        skyboxShader_.unbind();
    }
}

void Renderer::initTextures()
{
    PngImage taimage = PngImage("../resource/texture/minecraft_atlas.png");
    TextureAtlas taatlas = TextureAtlas(taimage, 16, 16, 0);
    taatlas.init(textureAtlas);

    PngImage sbimage = PngImage("../resource/texture/skybox.png");
    TextureAtlas sbatlas = TextureAtlas(sbimage, 2, 3, 1);
    sbatlas.init(skyboxTexture);
}

void Renderer::setRenderDistance(float f)
{
    projectionMatrix = glm::perspective(1.5f, 8.0f/6.0f, 0.01f, f);
}

void Renderer::deleteRenderable(const void* key) {
    if (renderables_.find(key) != renderables_.end())
    {
        glDeleteVertexArrays(1, &renderables_[key].vao);
        renderables_.erase(key);
    }
}

void Renderer::clearChunkRenderables(const void* key)
{
    if(chunkRenderables.find(key) != chunkRenderables.end())
    {
        for(auto tempR : chunkRenderables[key].renderables)
        {
            glDeleteVertexArrays(1, &tempR.vao);
        }
        chunkRenderables.erase(key);
    }
}

void Renderer::clearRenderables() {
    for(auto it = renderables_.begin(); it != renderables_.end(); ++it)
    {
        glDeleteVertexArrays(1, &it->second.vao);
        it = renderables_.erase(it);
    }
}

void Renderer::cleanUp()
{
    glDeleteTextures(1, &textureAtlas);
    glDeleteTextures(1, &skyboxTexture);
    for(auto it = chunkRenderables.begin(); it != chunkRenderables.end();)
    {
        for(auto temp : it->second.renderables)
        {
            glDeleteVertexArrays(1, &temp.vao);
        }
        it = chunkRenderables.erase(it);
    }
}

void Renderer::updateViewMatrix(glm::mat4 vm)
{
    viewMatrix = vm;
}

void Renderer::updateProjectionMatrix(glm::mat4 pm)
{
    projectionMatrix = pm;
}