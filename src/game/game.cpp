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
        .initItems = initItems,
        .initBlocks = initBlocks,
    };

    return mod;
}

void initItems() {

}

void initBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "texture.png",
    };
    wmac::blocks::add(block);
}

