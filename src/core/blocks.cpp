#define BLOCKS_CPP
#include "core/blocks.hpp"

#include "core/render.hpp"

namespace wmac::blocks {

void add(InitBlockInfo& p_block) {
    render::addTextureToAtlas(p_block);
    p_block.id = m_idCounter++;
    m_blocks.push_back(p_block);
}

void add(std::vector<InitBlockInfo>& p_blocks) {
    for (auto& block : p_blocks) {
        render::addTextureToAtlas(block);
        block.id = m_idCounter++;
    }
    m_blocks.insert(m_blocks.end(), p_blocks.begin(), p_blocks.end());
}

void addDefaultBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "texture.png",
    };
    add(block);
}

};