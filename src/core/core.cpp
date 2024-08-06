#include "./core.hpp"

using namespace wmac;

#define FPS 0

void Core::run() {
    init();
    while (!WindowShouldClose()) {
        update();
        BeginDrawing(); {
        ClearBackground(BLUE);
            BeginMode3D(camera); {
                draw3D();
            } EndMode3D();
            drawUI();
        } EndDrawing();
    }
    deinit();
}

void Core::init() {
    const std::string dir = GetWorkingDirectory();
    if (dir.ends_with("/bin")) {
        ChangeDirectory("../");
    };
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, TITLE);
    DisableCursor();
    render::initAtlas();
    blocks::addDefaultBlocks();
    
    #if FPS > 0
    SetTargetFPS(FPS);
    #endif
    texture = LoadTexture("texture.png");
    render::initMesh();
    world::init();
}

void Core::update() {
    vec3 movementDelta = getMovementDelta();
    vec3 rotationDelta = VEC3(GetMouseDelta(), 0.0f) * SENSITIVITY;
    UpdateCameraPro(&camera, movementDelta, rotationDelta, 0.0f);
    ticks::check();
}

vec3 Core::getMovementDelta() {
    vec3 movementDelta = {
        (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
        (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
    };
    return movementDelta * (IsKeyDown(KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * GetFrameTime();;
}

void Core::draw3D() {
    render::draw();
}

void Core::drawUI() {
    DrawRectangle(0, 0, 120, 40, BLACK);
    DrawFPS(10, 10);
}

void Core::deinit() {
    world::deinit();
    CloseWindow();
}

