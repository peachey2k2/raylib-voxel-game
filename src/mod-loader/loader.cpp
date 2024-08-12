#define LOADER_CPP
#include "./loader.hpp"
#include "engine/core.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

namespace wmac::loader {

const char* MODS_DIR = "mods";

const char* SHARED_LIB_EXT = 
#ifdef _WIN32
    ".dll";
#elif __linux__
    ".so";
#endif

void loadMods() {
    raylib::FilePathList modsPaths = raylib::LoadDirectoryFilesEx(MODS_DIR, SHARED_LIB_EXT, true);
    for (u32 i = 0; i < modsPaths.count; i++) {
        const char* modsPath = modsPaths.paths[i];
        loadMod(modsPath);
        printf("Loaded mod: %s\n", modsPath);
    }
}

void loadMod(const char* p_modPath) {
    const std::string modAbsPath = (core::getAppDir() + p_modPath);

    #ifdef _WIN32
    void* modLib = LoadLibrary(modAbsPath.c_str());
    #elif __linux__
    void* modLib = dlopen(modAbsPath.c_str(), RTLD_NOW);
    #endif
    
    ASSERT(modLib, "Failed to load mod: " + modAbsPath + '\n' + dlerror());

    Mod (*initFunc)();
    #ifdef _WIN32
    initFunc = (Mod (*)())GetProcAddress((HMODULE)modLib, "init");
    #elif __linux__
    initFunc = (Mod (*)())dlsym(modLib, "init");
    #endif

    ASSERT(initFunc, "Failed to find init function in mod: " + modAbsPath);

    Mod mod = initFunc();
    modList.push_back(mod);modList.push_back(mod);
}

void initFunctions() {
    ApiFunctions api = {
        .blocks__add = blocks::add,
    };
    for (auto& mod : modList) {
        if (mod.initFunctions) {
            mod.initFunctions(api);
        }
    }
}

void initItems() {
    for (auto& mod : modList) {
        if (mod.initItems) {
            mod.initItems();
        }
    }
}

void initBlocks() {
    for (auto& mod : modList) {
        if (mod.initBlocks) {
            mod.initBlocks();
        }
    }
}


}
