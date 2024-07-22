#pragma once

#include "misc/utils.hpp"
#include "core/blocks.hpp"

namespace wmac {

class Core {
    private:
        const char* TITLE = "Test";
        const u32 WIDTH = 800;
        const u32 HEIGHT = 600;

        const f32 SENSITIVITY = 0.1f;
        const f32 SPEED = 5.0f;

        Camera3D camera {
            .position = { 10.0f, 1.0f, 10.0f },
            .target = { 0.0f, 0.0f, 0.0f },
            .up = { 0.0f, 1.0f, 0.0f },
            .fovy = 60.0f,
            .projection = CAMERA_PERSPECTIVE
        };

        Texture2D texture;
        
    public:
        void run();

    private:
        void init();
        void update();
            vec3 getMovementDelta();
        void draw3D();
        void drawUI();
        void deinit();

};

};