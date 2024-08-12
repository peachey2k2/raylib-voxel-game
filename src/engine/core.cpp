#define CORE_CPP

#include "./core.hpp"

#include "./blocks.hpp"
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"

#include "mod-loader/loader.hpp"

namespace wmac::core {

#define FPS 0

void run() {
    init();
    while (!WindowShouldClose()) {
        update();
        BeginDrawing(); {
        ClearBackground(BLUE);
            BeginMode3D(m_camera); {
                draw3D();
            } EndMode3D();
            drawUI();
        } EndDrawing();
    }
    deinit();
}

void init() {
    initRaylib();
    
    loader::loadMods();
    loader::initFunctions();

    render::initAtlas();
    // blocks::addDefaultBlocks();
    loader::initBlocks();
    
    render::initMesh();
    world::init();
}

void initRaylib() {
    m_appDir = GetApplicationDirectory();
    ChangeDirectory(m_appDir.c_str());
    
    SetConfigFlags(
        // FLAG_WINDOW_RESIZABLE |
        FLAG_BORDERLESS_WINDOWED_MODE    
    );
    InitWindow(WIDTH, HEIGHT, TITLE);
    DisableCursor();
    #if FPS > 0
    SetTargetFPS(FPS);
    #endif
}

void update() {
    vec3 movementDelta = getMovementDelta();
    vec3 rotationDelta = VEC3(GetMouseDelta(), 0.0f) * SENSITIVITY;
    UpdateCameraPro(&m_camera, movementDelta, rotationDelta, 0.0f);
    ticks::check();
}

vec3 getMovementDelta() {
    vec3 movementDelta = {
        (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
    };
    return movementDelta * (IsKeyDown(KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * GetFrameTime();;
}

void draw3D() {
    DrawGrid(20, 1.0f);
    render::draw();
}

void drawUI() {
    DrawRectangle(0, 0, 120, 40, BLACK);
    DrawFPS(10, 10);
}

void deinit() {
    world::deinit();
    CloseWindow();
}

std::string getAppDir() {
    return m_appDir;
}

}
