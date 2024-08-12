#pragma once

#include "misc/utils.hpp"

#include "engine/blocks.hpp"

namespace wmac::loader {

#ifdef LOADER_CPP

extern const char* MODS_DIR;
extern const char* SHARED_LIB_EXT;

void loadMod(const char* modFile);

#endif

inline auto modList = std::vector<Mod>();

void loadMods();

void initFunctions();
void initItems();
void initBlocks();

}



