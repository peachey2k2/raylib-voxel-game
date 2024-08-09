#pragma once

#include "misc/utils.hpp"

namespace wmac::core {

inline const char* TITLE = "Test";
inline const u32 WIDTH = 800;
inline const u32 HEIGHT = 600;

inline const f32 SENSITIVITY = 0.1f;
inline const f32 SPEED = 5.0f;
 
inline raylib::Camera3D m_camera {
    .position = { 10.0f, 1.0f, 10.0f },
    .target = { 0.0f, 0.0f, 0.0f },
    .up = { 0.0f, 1.0f, 0.0f },
    .fovy = 60.0f,
    .projection = raylib::CAMERA_PERSPECTIVE,
};

#ifdef CORE_CPP

void init();
void update();
    vec3 getMovementDelta();
void draw3D();
void drawUI();
void deinit();

#endif

void run();


};
