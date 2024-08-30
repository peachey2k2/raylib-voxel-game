#pragma once

#include "utils/tools.hpp"
#include "classes/entity.hpp"

namespace wmac {

class Player : public Entity {

private:
    static Player* m_mainPlayer;

public:
    Player(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data = nullptr);

    Player* getMainPlayer() { return m_mainPlayer; }

};

}