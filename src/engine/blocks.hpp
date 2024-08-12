#pragma once

#include "misc/utils.hpp"

namespace wmac::blocks {

#ifdef BLOCKS_CPP

std::vector<Block> m_blocks(256);
u64 m_idCounter = 1; // 0 is reserved for air

#endif

void add(InitBlockInfo &p_block);

u64 getNewId();


};
