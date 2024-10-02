#define CORE_CPP

#include "./core.hpp"

#include "utils/tools.hpp"

#include "./blocks.hpp"
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"
#include "./ui.hpp"
#include "classes/player.hpp"

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
    while (m_isRunning) {
        // TODO: add more stuff here
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    deinit();
}

void renderLoop() {
    initRenderer();
    doChecks();
    world::init(); // temporary, we need to wait for the window before this (for some reason)
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
    m_isRunning = false;
}

void init() {
    #ifdef __linux__
    setEnvVars();
    #endif

    initRaylib();

    loader::loadMods();
    loader::initFunctions();

    render::initAtlas();
    // blocks::addDefaultBlocks();
    loader::initBlocks();
    
    // world::init();
    m_worldThread = std::thread(world::threadLoop);
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

const std::string CHECK = "[✓]";
const std::string CROSS = "[✗]";
void doChecks() {
    i32 temp_i32;
    bool res;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &temp_i32);
    res = temp_i32 >= 4096;
    tools::say(res ? CHECK : CROSS, "Max texture size:", temp_i32, "(needs to be at least 4096)");
    if (not res) {
        tools::say("Please update your graphics drivers.");
        exit(1);
    }
}

void initRenderer() {
    InitWindow(WIDTH, HEIGHT, TITLE);
    m_window = glfwGetCurrentContext(); (void)m_window;
    
    m_font = LoadFont("res/fonts/Miracode.ttf");
    DisableCursor();
    #if FPS > 0
    SetTargetFPS(FPS);
    #endif
    render::initMesh();
    ui::init();
}

void loadCoreMod() {
    ModInfo coreModInfo = {
        .name = "core",
        .version = "0.1",
        .author = "peachey2k2",
        .description = "The core mod",
    };
    Mod coreMod = {
        .info = coreModInfo,
        .initFunctions = nullptr,
        .initItems = initItems,
        .initBlocks = initBlocks,
        .initEntities = initEntities,
    };
}

void initItems() {

}

void initBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "res/blocks/grass.png",
    };
    wmac::blocks::add(block);
}

void initEntities() {

}

void update() {
    updatePlayer();

    vec3i newChunk = world::getChunkLoc(vec3(m_position));
    if (newChunk != m_chunk) {
        // tools::say(m_chunk, newChunk);
        m_chunk = newChunk;
        // world::generateChunksAt(m_chunk, RENDER_DISTANCE);
        world::m_chunksToGenerateAt.push(m_chunk);
    }
    // ticks::check();
}

void updatePlayer() {
    Player* player = Player::getMainPlayer();
    if (player == nullptr) {
        new Player(true, {0, 0, 0}, nullptr);
        Player::getMainPlayer()->updateMovementDelta({0, 20, 0});
        updatePlayer();
        return;
    }

    vec3d movementDelta = getMovementDelta();
    vec3 rotationDelta = vec3(GetMouseDelta(), 0.0) * SENSITIVITY;
    Player::getMainPlayer()->updateMovementDelta(movementDelta);

    UpdateCameraPro(&m_camera, vec3::zero, rotationDelta, 0.0f);
    Player::getMainPlayer()->updateRotation(m_camera.target - m_camera.position);
}

void moveCamera(vec3 p_pos) {
    vec3 delta = p_pos - m_camera.position;
    m_camera.position = p_pos;
    m_camera.target = m_camera.target + delta;
}

void updateVision() {
}

vec3d getMovementDelta() {
    // vec3i movementDelta = {
    //     (i32)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
    //     (i32)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
    //     (i32)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
    // };
    vec2 inputDir = {
        (f32)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
        (f32)(IsKeyDown(KEY_A) - IsKeyDown(KEY_D)),
    };
    vec3 lookDir = Player::getMainPlayer()->getDir();
    lookDir.y = 0;
    lookDir = lookDir.normalize();
    vec3 movementDelta = {
        inputDir.x * lookDir.x + inputDir.y * lookDir.z,
        0,
        inputDir.x * lookDir.z - inputDir.y * lookDir.x,
    };

    return movementDelta * (IsKeyDown(KEY_LEFT_CONTROL) ? 4.0f : 1.0f) * SPEED * GetFrameTime();
}

void draw3D() {
    render::update();
}

void drawUI() {
    DrawRectangle(0, 0, 140, 80, BLACK);
    DrawFPS(10, 10);
    DrawTextEx(m_font, TextFormat("x: %.3f (%d)\ny: %.3f (%d)\nz: %.3f (%d)", m_position.x, m_chunk.x, m_position.y, m_chunk.y, m_position.z, m_chunk.z), {10, 30}, 16, 0, WHITE);

    ui::draw();
}

void deinit() {
    m_tickWorld = false;
    m_generateTerrain = false;
    m_ticksThread.join();
    m_renderThread.join();
    m_worldThread.join();
    CloseWindow();
    world::deinit();
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

bool terrainShouldGenerate() {
    return m_generateTerrain;
}

};

