#pragma once

#include "glm/glm.hpp"

#include "util/bounding_box.hpp"

struct Plane {
    float coords[4];
};

/*
 * FrustumCulling struct contains functions to test frustum culling against boundingboxes
 * Holds 6 planes which must initialized with viewProjection matrix before every rendering cycle
 */

struct FrustumCulling {
    glm::vec3 lookUp[6] = { glm::vec3(), glm::vec3(),
                            glm::vec3(), glm::vec3(),
                            glm::vec3(), glm::vec3() };
    bool isInsideFrustum(BoundingBox bb);  // Tests bounding box against current planes
    void getFrustumPlanes(glm::mat4 vpM);   // Extracts planes needed in frustum testing
    
    // Frustum planes
    Plane planes[6];
};