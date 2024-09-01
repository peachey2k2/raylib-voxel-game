#pragma once

#include "utils/tools.hpp"

namespace wmac {

class Entity {

protected:
    bool m_active = true;
    vec3d m_pos, m_nextPos;
    vec3 m_dir;

    vec3d m_velocity;
    vec3d m_nextMovement;
    std::mutex m_movementMutex;
    
    u64 m_id;

public:
    Entity(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data = nullptr);

    void updateRotation(const vec3 p_dir);
    void updateMovementDelta(const vec3d p_deltaMovement);

    vec3d applyMovement();
};
    
}