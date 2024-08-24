#define CORE_CPP

#include "./core.hpp"

#include "./blocks.hpp"
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"

#include "mod-loader/loader.hpp"

namespace wmac::core {

#define FPS 0

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#elif __linux__
    void setEnvVars() {
        setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
        setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
    }
#elif __APPLE__
// UNLUCKY :D
#endif

void run() {
    init();
    m_renderThread = std::thread(renderLoop);
    m_ticksThread = std::thread(ticks::checkLoop);
    while (not WindowShouldClose()) {
        // TODO: add more stuff here
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    deinit();
}

void renderLoop() {
    initRenderer();
    while (not WindowShouldClose()) {
        update();
        BeginDrawing();
            ClearBackground(BLUE);
            BeginMode3D(m_camera);
                draw3D();
            EndMode3D();
            drawUI();
        EndDrawing();
        frameCount++;
    }
}

void init() {
    #ifdef __linux__
    setEnvVars();
    #endif
    initRaylib();
    // m_window = glfwGetCurrentContext();
    
    loader::loadMods();
    loader::initFunctions();

    render::initAtlas();
    // blocks::addDefaultBlocks();
    loader::initBlocks();
    
    world::init();
}

void initRaylib() {
    m_appDir = GetApplicationDirectory();
    ChangeDirectory(m_appDir.c_str());
    
    SetConfigFlags(
        // FLAG_VSYNC_HINT |
        // FLAG_FULLSCREEN_MODE |
        // FLAG_WINDOW_RESIZABLE |
        // FLAG_WINDOW_UNDECORATED |
        // FLAG_WINDOW_HIDDEN |
        // FLAG_WINDOW_MINIMIZED |
        // FLAG_WINDOW_MAXIMIZED |
        // FLAG_WINDOW_UNFOCUSED |
        // FLAG_WINDOW_TOPMOST |
        // FLAG_WINDOW_ALWAYS_RUN |
        // FLAG_WINDOW_TRANSPARENT |
        // FLAG_WINDOW_HIGHDPI |
        // FLAG_WINDOW_MOUSE_PASSTHROUGH |
        FLAG_BORDERLESS_WINDOWED_MODE |
        // FLAG_MSAA_4X_HINT |
        // FLAG_INTERLACED_HINT |
    0);
}

void initRenderer() {
    InitWindow(WIDTH, HEIGHT, TITLE);
    
    m_font = LoadFont("res/fonts/Miracode.ttf");
    DisableCursor();
    #if FPS > 0
    SetTargetFPS(FPS);
    #endif
    render::initMesh();
}

void update() {
    vec3 movementDelta = getMovementDelta();
    vec3 rotationDelta = VEC3(GetMouseDelta(), 0.0f) * SENSITIVITY;
    UpdateCameraPro(&m_camera, movementDelta, rotationDelta, 0.0f);

    vec3i newChunk = world::getChunkLoc(m_position);
    if (newChunk != m_chunk) {
        m_chunk = newChunk;
        world::generateChunksAt(m_chunk, RENDER_DISTANCE);
    }

    // ticks::check();
}

vec3 getMovementDelta() {
    vec3 movementDelta = {
        (f32)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
        (f32)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (f32)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
    };
    return movementDelta * (IsKeyDown(KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * GetFrameTime();;
}

void draw3D() {
    DrawGrid(20, 1.0f);
    render::update();
}

void drawUI() {
    DrawRectangle(0, 0, 140, 80, BLACK);
    DrawFPS(10, 10);
    DrawTextEx(m_font, TextFormat("x: %.3f (%d)\ny: %.3f (%d)\nz: %.3f (%d)", m_position.x, m_chunk.x, m_position.y, m_chunk.y, m_position.z, m_chunk.z), {10, 30}, 16, 0, WHITE);
}

void deinit() {
    m_tickWorld = false;
    world::deinit();
    m_ticksThread.join();
    m_renderThread.join();
    CloseWindow();
}

std::string getAppDir() {
    return m_appDir;
}

u64 getFrameCount() {
    return frameCount;
}

bool worldShouldTick() {
    return m_tickWorld;
}

}
