#pragma once

#include "utils/utils.hpp"

namespace wmac::core {

inline const char* TITLE = "Test";
inline const u32 WIDTH = 800;
inline const u32 HEIGHT = 600;

inline const f32 SENSITIVITY = 0.1f;
inline const f32 SPEED = 5.0f;

inline const u32 RENDER_DISTANCE = 16;

#ifdef CORE_CPP

bool m_tickWorld = true;
bool m_generateTerrain = true;

// #include <GLFW/glfw3.h>
// GLFWwindow* m_window;
Camera3D m_camera {
    .position = { 10.0f, 1.0f, 10.0f },
    .target = { 0.0f, 0.0f, 0.0f },
    .up = { 0.0f, 1.0f, 0.0f },
    .fovy = 60.0f,
    .projection = CAMERA_PERSPECTIVE,
};
vec3i m_chunk;
vec3& m_position = m_camera.position;
std::string m_appDir;
Font m_font;

u64 frameCount = 0;

std::thread m_ticksThread; // for per-tick callbacks
std::thread m_renderThread; // for rendering and mesh updates
std::thread m_worldThread; // for world generation

void init();
    void initRaylib();
void renderLoop();
    void initRenderer();
    void draw3D();
    void drawUI();
void update();
    vec3 getMovementDelta();
void deinit();

#endif

void run();

std::string getAppDir();
u64 getFrameCount();
bool worldShouldTick();
bool terrainShouldGenerate();

};
