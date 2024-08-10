#pragma once
#define WMAC_API extern "C"

WMAC_API typedef struct ModInfo {
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

WMAC_API typedef struct Mod {
    ModInfo info;
    void (*initItems)();
    void (*initBlocks)();
} Mod;

typedef struct InitBlockInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
} InitBlockInfo;

namespace wmac::blocks {
    WMAC_API void add(InitBlockInfo &p_block);
}
