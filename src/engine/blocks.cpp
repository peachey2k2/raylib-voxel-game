#define BLOCKS_CPP
#include "./blocks.hpp"

#include "./render.hpp"
#include "mod-loader/loader.hpp"

namespace wmac::blocks {

void add(ModID modId, InitBlockInfo& p_block) {
    std::string texPath = loader::getModPath(modId) + p_block.texture;
    m_blocks.push_back(
        Block {
            .itemId = getNewId(),
            .textureId = render::addTextureToAtlas(texPath.c_str()),
            .name = p_block.name,
            .tooltip = p_block.tooltip,
            .texture = p_block.texture,
        }
    );
}

u64 getNewId() {
    return m_idCounter++;
}

};