#pragma once

#include "utils.hpp"

namespace wmac::blocks {
    // WMAC_API void add(InitBlockInfo &p_block);
    void (*add)(ModID modId, InitBlockInfo &p_block);
}

#define BIND_FUNCTIONS(api) \
    wmac::blocks::add = api.blocks__add;



