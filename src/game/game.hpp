#pragma once

#include "../misc/utils.hpp"
#include "../misc/api.hpp"

extern "C" Mod init();

void initFunctions(ApiFunctions &p_api);
void initItems();
void initBlocks();
