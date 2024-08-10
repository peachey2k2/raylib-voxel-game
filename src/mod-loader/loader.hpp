#pragma once

#include "misc/utils.hpp"

extern "C" {
    typedef struct ModInfo {
        const char* name;
        const char* version;
        const char* author;
        const char* description;
        const char* license;
        const char* source;
        const char* dependencies;
        const char* conflicts;
        const int loadPriority;
    } ModInfo;

    typedef struct Mod {
        ModInfo info;
        void (*initItems)();
        void (*initBlocks)();
    } Mod;
}

namespace wmac::loader {

#ifdef LOADER_CPP

extern const char* MODS_DIR;
extern const char* SHARED_LIB_EXT;

void loadMod(const char* modFile);

#endif

inline auto modList = std::vector<Mod>();

void loadMods();

void initItems();
void initBlocks();

}

