#include "./core.hpp"

using namespace wmac;

#define FPS 0

void Core::run() {
    init();
    while (!raylib::WindowShouldClose()) {
        update();
        raylib::BeginDrawing(); {
        raylib::ClearBackground(raylib::BLUE);
            raylib::BeginMode3D(camera); {
                draw3D();
            } raylib::EndMode3D();
            drawUI();
        } raylib::EndDrawing();
    }
    deinit();
}

void Core::init() {
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
    texture = raylib::LoadTexture("texture.png");
    render::initMesh();
    world::init();
}

void Core::update() {
    vec3 movementDelta = getMovementDelta();
    vec3 rotationDelta = VEC3(raylib::GetMouseDelta(), 0.0f) * SENSITIVITY;
    UpdateCameraPro(&camera, movementDelta, rotationDelta, 0.0f);
    ticks::check();
}

vec3 Core::getMovementDelta() {
    vec3 movementDelta = {
        (float)(raylib::IsKeyDown(raylib::KEY_W) - raylib::IsKeyDown(raylib::KEY_S)),
        (float)(raylib::IsKeyDown(raylib::KEY_D) - raylib::IsKeyDown(raylib::KEY_A)),
        (float)(raylib::IsKeyDown(raylib::KEY_SPACE) - raylib::IsKeyDown(raylib::KEY_LEFT_SHIFT)),
    };
    return movementDelta * (raylib::IsKeyDown(raylib::KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * raylib::GetFrameTime();;
}

void Core::draw3D() {
    raylib::DrawGrid(20, 1.0f);
    render::draw();
}

void Core::drawUI() {
    raylib::DrawRectangle(0, 0, 120, 40, raylib::BLACK);
    raylib::DrawFPS(10, 10);
}

void Core::deinit() {
    world::deinit();
    raylib::CloseWindow();
}

