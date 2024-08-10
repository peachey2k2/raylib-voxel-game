#define BLOCKS_CPP
#include "./blocks.hpp"

#include "./render.hpp"

namespace wmac::blocks {

void add(InitBlockInfo& p_block) {
    m_blocks.push_back(
        Block {
            .itemId = getNewId(),
            .textureId = render::addTextureToAtlas(p_block.texture),
            .name = p_block.name,
            .tooltip = p_block.tooltip,
            .texture = p_block.texture,
        }
    );
}

template <typename Container>
void add(Container& p_blocks) {
    for (auto& block : p_blocks) {
        add(block);
    }
}

void addDefaultBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "texture.png",
    };
    add(block);
}

u64 getNewId() {
    return m_idCounter++;
}

};