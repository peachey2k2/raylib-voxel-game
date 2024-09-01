#include "classes/player.hpp"

namespace wmac {

Player::Player(const bool p_active, const vec3d& p_pos, const PlayerData* p_data)
    : Entity(p_active, p_pos, 0, p_data) {
    
    m_mainPlayer = this;

    
}



}