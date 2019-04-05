#include "util/bounding_box.hpp"

// STL
#include <iostream>

bool BoundingBox::intersectsRay(Ray& r) const {
    glm::vec3 dirfrac;
    dirfrac.x = r.d.x != 0 ? 1.0f / r.d.x : FLT_MAX;
    dirfrac.y = r.d.y != 0 ? 1.0f / r.d.y : FLT_MAX;
    dirfrac.z = r.d.z != 0 ? 1.0f / r.d.z : FLT_MAX;

    float t1 = (min.x - r.o.x) * dirfrac.x;
    float t2 = (max.x - r.o.x) * dirfrac.x;
    float t3 = (min.y - r.o.y) * dirfrac.y;
    float t4 = (max.y - r.o.y) * dirfrac.y;
    float t5 = (min.z - r.o.z) * dirfrac.z;
    float t6 = (max.z - r.o.z) * dirfrac.z;

    float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    if (tmax < 0) {
        return false;
    }

    if (tmin > tmax) {
        return false;
    }

    r.t = tmin;
    return true;
}

std::array<BoundingBox, 8> BoundingBox::partition() const {
    using glm::vec3;
    vec3 mid = (max + min) / 2.0f;

    std::array<BoundingBox, 8> ret{{
        BoundingBox{vec3(min.x, min.y, min.z), vec3(mid.x, mid.y, mid.z)},  // (0,0,0)
        BoundingBox{vec3(min.x, min.y, mid.z), vec3(mid.x, mid.y, max.z)},  // (0,0,1)
        BoundingBox{vec3(min.x, mid.y, min.z), vec3(mid.x, max.y, mid.z)},  // (0,1,0)
        BoundingBox{vec3(min.x, mid.y, mid.z), vec3(mid.x, max.y, max.z)},  // (0,1,1)
        BoundingBox{vec3(mid.x, min.y, min.z), vec3(max.x, mid.y, mid.z)},  // (1,0,0)
        BoundingBox{vec3(mid.x, min.y, mid.z), vec3(max.x, mid.y, max.z)},  // (1,0,1)
        BoundingBox{vec3(mid.x, mid.y, min.z), vec3(max.x, max.y, mid.z)},  // (1,1,0)
        BoundingBox{vec3(mid.x, mid.y, mid.z), vec3(max.x, max.y, max.z)},  // (1,1,1)
    }};

    return ret;
}