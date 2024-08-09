#define CORE_CPP

#include "core/core.hpp"

#include "core/blocks.hpp"
#include "core/render.hpp"
#include "core/ticks.hpp"
#include "core/world.hpp"

namespace wmac::core {

#define FPS 0

void run() {
    init();
    while (!raylib::WindowShouldClose()) {
        update();
        raylib::BeginDrawing(); {
        raylib::ClearBackground(raylib::BLUE);
            raylib::BeginMode3D(m_camera); {
                draw3D();
            } raylib::EndMode3D();
            drawUI();
        } raylib::EndDrawing();
    }
    deinit();
}

void init() {
    const std::string dir = raylib::GetWorkingDirectory();
    if (dir.ends_with("/bin")) {
        raylib::ChangeDirectory("../");
    };
    
    raylib::SetConfigFlags(raylib::FLAG_WINDOW_RESIZABLE);
    raylib::InitWindow(WIDTH, HEIGHT, TITLE);
    raylib::DisableCursor();
    render::initAtlas();
    blocks::addDefaultBlocks();
    
    #if FPS > 0
    SetTargetFPS(FPS);
    #endif
    render::initMesh();
    world::init();
}

void update() {
    vec3 movementDelta = getMovementDelta();
    vec3 rotationDelta = VEC3(raylib::GetMouseDelta(), 0.0f) * SENSITIVITY;
    UpdateCameraPro(&m_camera, movementDelta, rotationDelta, 0.0f);
    ticks::check();
}

vec3 getMovementDelta() {
    vec3 movementDelta = {
        (float)(raylib::IsKeyDown(raylib::KEY_W) - raylib::IsKeyDown(raylib::KEY_S)),
        (float)(raylib::IsKeyDown(raylib::KEY_D) - raylib::IsKeyDown(raylib::KEY_A)),
        (float)(raylib::IsKeyDown(raylib::KEY_SPACE) - raylib::IsKeyDown(raylib::KEY_LEFT_SHIFT)),
    };
    return movementDelta * (raylib::IsKeyDown(raylib::KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * raylib::GetFrameTime();;
}

void draw3D() {
    raylib::DrawGrid(20, 1.0f);
    render::draw();
}

void drawUI() {
    raylib::DrawRectangle(0, 0, 120, 40, raylib::BLACK);
    raylib::DrawFPS(10, 10);
}

void deinit() {
    world::deinit();
    raylib::CloseWindow();
}

}
