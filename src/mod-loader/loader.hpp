#pragma once

#include "utils/utils.hpp"

#include "engine/blocks.hpp"

namespace wmac::loader {

#ifdef LOADER_CPP

extern const char* MODS_DIR;
extern const char* SHARED_LIB_EXT;

ModID m_currentMod;

void loadMod(const char* modFile, ModID id);

#endif

inline auto modList = std::vector<Mod>();

void loadMods();

void initFunctions();
void initItems();
void initBlocks();

std::string getModPath(ModID id);
ModID getCurrentMod();

}



