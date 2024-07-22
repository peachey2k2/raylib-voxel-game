#include "./core.hpp"

using namespace wmac;

#define FPS 60

void Core::run() {
    init();
    while (!WindowShouldClose()) {
        update();
        BeginDrawing(); {
        ClearBackground(BLACK);
            BeginMode3D(camera); {
                draw3D();
            } EndMode3D();
            drawUI();
        } EndDrawing();
    }
    deinit();
}

void Core::init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, TITLE);
    DisableCursor();
    blocks::createAtlas();
    blocks::addDefaultBlocks();
    
    #if FPS > 0
    // SetTargetFPS(60);
    texture = LoadTexture("texture.png");
    blocks::init();
    #endif
}

void Core::update() {
    vec2 mouseDelta = GetMouseDelta();
    vec3 movementDelta = getMovementDelta();
    UpdateCameraPro(&camera, movementDelta, VEC3(mouseDelta, 0.0f) * SENSITIVITY, 0.0f);
    blocks::update();
}

vec3 Core::getMovementDelta() {
    vec3 movementDelta = {
        (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
    };
    return movementDelta * (IsKeyDown(KEY_LEFT_CONTROL) ? 2.0f : 1.0f) * SPEED * GetFrameTime();;
}

void Core::draw3D() {
    DrawGrid(10, 1.0f);
    blocks::draw();
}

void Core::drawUI() {
    DrawFPS(10, 10);
}

void Core::deinit() {
    CloseWindow();
}

