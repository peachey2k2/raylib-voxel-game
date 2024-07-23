#pragma once

#include "misc/utils.hpp"

namespace wmac {

typedef struct InitBlockInfo {
    u64 id;
    const char* name;
    const char* tooltip;
    const char* texture;
    const char* model;
    vec2 texCoords;
} InitBlockInfo;

};

namespace wmac::blocks {

#ifdef BLOCKS_CPP

std::vector<InitBlockInfo> blocks(256);
u64 idCounter = 1; // 0 is reserved for air

#endif

void add(InitBlockInfo &p_block);
void add(std::vector<InitBlockInfo> &p_blocks);

void addDefaultBlocks();


};
