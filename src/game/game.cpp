#include "./game.hpp"

extern "C" Mod init() {
    ModInfo info = {
        .name = "World Eater",
        .version = "0.0.1",
        .author = "peachey2k2",
        .license = "MIT",
    };

    Mod mod = {
        .info = info,
        .initFunctions = initFunctions,
        .initItems = initItems,
        .initBlocks = initBlocks,
    };

    return mod;
}

void initFunctions(ApiFunctions &p_api) {
    BIND_FUNCTIONS(p_api);
}

void initItems() {
}

void initBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "../texture.png",
    };
    wmac::blocks::add(block);
}

