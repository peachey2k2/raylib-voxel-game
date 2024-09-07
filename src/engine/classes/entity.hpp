#pragma once

#include "utils/tools.hpp"

namespace wmac {

class Entity {

protected:
    bool m_active = true;
    Range<vec3d> m_collisionBox;

    vec3d m_pos, m_nextPos;
    vec3 m_dir;

    vec3d m_velocity;
    std::mutex m_movementMutex1;
    std::mutex m_movementMutex2;
    vec3d m_nextMovement[2] = { {0, 0, 0}, {0, 0, 0} };
    i32 m_movementIndex = 0;
    
    u64 m_id;

public:
    Entity(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data = nullptr);

    void updateRotation(const vec3 p_dir);
    void updateMovementDelta(const vec3d p_deltaMovement);

    Range<vec3d> getCollisionBox() { return m_collisionBox; };
    vec3d getPos() { return m_pos; };

    // IMPORTANT: these are only meant to be used for the physics engine.
    vec3d resetMovementDelta();
    void setPosition(const vec3d p_pos) { m_pos = p_pos; };
};
    
}