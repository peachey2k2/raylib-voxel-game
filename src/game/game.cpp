#include "./game.hpp"

extern "C" Mod init() {
    ModInfo info = {
        .name = "World Eater",
        .author = "peachey2k2",
        .license = "MIT",
        .version = "0.0.1",
    };

    Mod mod = {
        .info = info,
        .initItems = initItems,
        .initBlocks = initBlocks,
    };


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

