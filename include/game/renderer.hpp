#pragma once

// STL
#include <vector>
#include <map>

// Own
#include "game/shader/chunkshader.hpp"
#include "game/shader/skyboxshader.hpp"
#include "game/shader/watershader.hpp"
#include "game/shader_program.hpp"
#include "util/renderable.hpp"
#include "util/mesh.hpp"
#include "util/frustumculling.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <map>

class World;

class Renderer {
    public:
        Renderer();

        void init();
        void cleanUp();
        void initTextures();
        void updateRenderable(const void* key, const Renderable& renderable);
        void deleteRenderable(const void* key);    // Deletes 1 renderable
        void clearRenderables();                                     // Clears all renderables

        void updateChunkRenderables(const void* key, const RenderableContainer& renderable);
        void clearChunkRenderables(const void* key);
        void updateChunkRenderable(const void* key, const Renderable& renderable);

        /*
        / render(world, dt, waterdt) renders the whole world
        / dt [0.0 - 1.0] is used to calculate new viewMatrix
        / waterdt [0.0, 2pi] is used in watershader to generate waves
        */
        void render(World* world, float dt, float waterdt);
        void renderChunks(World* world);
        void renderSkyBox(World* world);
        void renderChunkRenderables(World* world, float waterdt);
        void renderSolidMeshes(World* world, std::vector<Renderable> rend);
        void renderWaterMeshes(World* world, std::vector<Renderable> rend, float dt);

        void updateViewMatrix(glm::mat4 vm);
        void updateProjectionMatrix(glm::mat4 pm);
        void updateViewProjection();
        void setRenderDistance(float f);

    private:
        std::map<const void*, Renderable> renderables_;
        std::map<const void*, RenderableContainer> chunkRenderables;

        ChunkShader    chunkShader_;
        WaterShader    waterShader_;
        SkyBoxShader   skyboxShader_;

        GLuint         textureAtlas, skyboxTexture;

        FrustumCulling frustumCulling_;
        glm::mat4      viewMatrix;
        glm::mat4      projectionMatrix;
};