#include "./player.hpp"

namespace wmac {

Player* Player::m_mainPlayer = nullptr;

Player::Player(const bool p_active, const vec3d& p_pos, const PlayerData* p_data)
    : Entity(p_active, p_pos, 0, p_data) {
    
    m_mainPlayer = this;
    m_collisionBox = { {-0.4, 0, -0.4}, {0.4, 1.8, 0.4} }; 

    
}



}