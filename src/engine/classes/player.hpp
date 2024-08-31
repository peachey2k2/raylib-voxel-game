#pragma once

#include "utils/tools.hpp"
#include "classes/entity.hpp"

namespace wmac {

class Player : public Entity {

private:
    static Player* m_mainPlayer;

public:
    Player(const bool p_active, const vec3d& p_pos, const PlayerData* p_data);

    vec3 updateMovementDelta(const vec3d p_deltaMovement, const vec3 p_deltaRotation);

    static Player* getMainPlayer() { return m_mainPlayer; }

};

}