#include "classes/player.hpp"

namespace wmac {

Player::Player(const bool p_active, const vec3d& p_pos, const PlayerData* p_data)
    : Entity(p_active, p_pos, 0, p_data) {
    
    m_mainPlayer = this;

    
}

vec3 Player::updateMovementDelta(const vec3d p_deltaMovement, const vec3 p_deltaRotation) {
    m_movementMutex.lock();
    m_nextMovement += p_deltaMovement;
    m_movementMutex.unlock();
}

}