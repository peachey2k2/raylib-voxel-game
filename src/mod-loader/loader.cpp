#define LOADER_CPP
#include "./loader.hpp"

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
    raylib::FilePathList modsFiles = raylib::LoadDirectoryFilesEx(MODS_DIR, SHARED_LIB_EXT, true);
    for (u32 i = 0; i < modsFiles.count; i++) {
        const char* modFile = modsFiles.paths[i];
        loadMod(modFile);
    }
}

void loadMod(const char* modFile) {
    #ifdef _WIN32
    void* modLib = LoadLibrary(modFile);
    #elif __linux__
    void* modLib = dlopen(modFile, RTLD_NOW);
    #endif
    
    ASSERT(modLib, std::string("Failed to load mod: ") + modFile);

    Mod (*initFunc)();
    #ifdef _WIN32
    initFunc = (Mod (*)())GetProcAddress((HMODULE)modLib, "init");
    #elif __linux__
    initFunc = (Mod (*)())dlsym(modLib, "init");
    #endif

    ASSERT(initFunc, std::string("Failed to find init function in mod: ") + modFile);

    Mod mod = initFunc();
    modList.push_back(mod);
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
