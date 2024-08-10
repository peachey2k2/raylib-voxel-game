#pragma once

#include "misc/utils.hpp"

namespace wmac {

typedef struct InitBlockInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
} InitBlockInfo;

typedef struct Block {
    u64 itemId;
    u32 textureId;
    const char* name;
    const char* tooltip;
    const char* texture;
} Block;

};

namespace wmac::blocks {

#ifdef BLOCKS_CPP

std::vector<Block> m_blocks(256);
u64 m_idCounter = 1; // 0 is reserved for air

#endif

void add(InitBlockInfo &p_block);

u64 getNewId();


};
