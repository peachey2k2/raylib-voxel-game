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
    core::loadCoreMod();
    FilePathList modsPaths = LoadDirectoryFilesEx(MODS_DIR, SHARED_LIB_EXT, true);
    for (u32 i = 0; i < modsPaths.count; i++) {
        ModID id = modList.size();
        const char* modsPath = modsPaths.paths[i];
        loadMod(modsPath, id);
        printf("Loaded mod: %s\n", modsPath);
    }
    
    for (u32 i = 0; i < modsPaths.capacity; i++) {
        free(modsPaths.paths[i]);
    }
    free(modsPaths.paths);
}

void loadMod(const char* p_modPath, ModID p_id) {
    const std::string modAbsPath = (core::getAppDir() + p_modPath);

    #ifdef _WIN32
    void* modLib = LoadLibrary(modAbsPath.c_str());
    #elif __linux__
    void* modLib = dlopen(modAbsPath.c_str(), RTLD_NOW);
    #endif
    
    ASSERT(modLib, "Failed to load mod: " + modAbsPath + '\n' + dlerror());

    Mod (*initFunc)(ModID);
    #ifdef _WIN32
    initFunc = (Mod (*)(ModID))GetProcAddress((HMODULE)modLib, "init");
    #elif __linux__
    initFunc = (Mod (*)(ModID))dlsym(modLib, "init");
    #endif

    ASSERT(initFunc, "Failed to find init function in mod: " + modAbsPath);

    Mod mod = initFunc(p_id);
    mod.path = modAbsPath.substr(0, modAbsPath.find_last_of('/') + 1);
    modList.push_back(mod);
}

void initFunctions() {
    ApiFunctions api = {
        .blocks__add = blocks::add,
    };
    for (ModID id=0; id<modList.size(); id++) {
        Mod& mod = modList[id];
        if (mod.initFunctions) {
            mod.initFunctions(api);
        }
    }
}

void initItems() {
    for (ModID id=0; id<modList.size(); id++) {
        Mod& mod = modList[id];
        m_currentMod = id;
        if (mod.initItems) {
            mod.initItems();
        }
    }
}

void initBlocks() {
    for (ModID id=0; id<modList.size(); id++) {
        Mod& mod = modList[id];
        m_currentMod = id;
        if (mod.initBlocks) {
            mod.initBlocks();
        }
    }
}

std::string getModPath(ModID p_id) {
    return modList[p_id].path;
}

ModID getCurrentMod() {
    return m_currentMod;
}

}
