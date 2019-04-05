#include "util/frustumculling.hpp"

void FrustumCulling::getFrustumPlanes(glm::mat4 vpM)
{
    for (int i = 0; i < 4; ++i) planes[0].coords[i] = vpM[i][3] + vpM[i][0];
    for (int i = 0; i < 4; ++i) planes[1].coords[i] = vpM[i][3] - vpM[i][0];
    for (int i = 0; i < 4; ++i) planes[2].coords[i] = vpM[i][3] + vpM[i][1];
    for (int i = 0; i < 4; ++i) planes[3].coords[i] = vpM[i][3] - vpM[i][1];
    for (int i = 0; i < 4; ++i) planes[4].coords[i] = vpM[i][3] + vpM[i][2];
    for (int i = 0; i < 4; ++i) planes[5].coords[i] = vpM[i][3] - vpM[i][2];
}

bool FrustumCulling::isInsideFrustum(BoundingBox bb) 
{
    for(int i = 0; i < 6; ++i)
    {

        int out = 0;
        glm::vec4 temp = glm::vec4(planes[i].coords[0], planes[i].coords[1], planes[i].coords[2], planes[i].coords[3]);
        out += ((glm::dot(temp, glm::vec4(bb.min.x, bb.min.y, bb.min.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.max.x, bb.min.y, bb.min.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.min.x, bb.max.y, bb.min.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.max.x, bb.max.y, bb.min.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.min.x, bb.min.y, bb.max.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.max.x, bb.min.y, bb.max.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.min.x, bb.max.y, bb.max.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        out += ((glm::dot(temp, glm::vec4(bb.max.x, bb.max.y, bb.max.z, 1.0f) ) < 0.0 ) ? 1 : 0);
        if(out == 8) return false;
    }
    return true;
}