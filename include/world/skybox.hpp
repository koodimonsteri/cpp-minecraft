#pragma once

#include <string>
#include "util/renderable.hpp"
#include "util/mesh.hpp"

class SkyBox {
    public:
        SkyBox();
        ~SkyBox();

        void init();

        Renderable getRenderable() const;
        void       setScale(float s);
    private:
        Renderable renderable_;
        float scale_ = 100;
};