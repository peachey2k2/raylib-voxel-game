#include "./entity.hpp"

#include "engine/entities.hpp"
#include "engine/core.hpp"
#include "engine/physics.hpp"

namespace wmac {

Entity* Entity::m_cameraAttachedEntity = nullptr;

Entity::Entity(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data) {
    m_active = p_active;
    m_pos = p_pos;
    m_id = p_id;
    
    (m_active ? entities::m_activeEntities : entities::m_inactiveEntities).push_back(this);
    // TODO: implement
    if (p_data != nullptr) {}
}

void Entity::updateRotation(const vec3 p_dir) {
    m_dir = p_dir.normalize();
}

void Entity::updateMovementDelta(const vec3d p_deltaMovement) {
    if (p_deltaMovement == vec3d::zero) return;
    const std::lock_guard<std::mutex> lock(m_movementMutex1);
    vec3d moveSum = m_nextMovement[m_movementIndex] + p_deltaMovement;
    m_nextMovement[m_movementIndex] = physics::collideMove(m_pos, moveSum, m_collisionBox);
    if (m_cameraAttachedEntity == this) {
        core::moveCamera(m_pos);
    }
}

vec3d Entity::resetMovementDelta() {
    const std::lock_guard<std::mutex> lock(m_movementMutex2);
    m_movementIndex = 1 - m_movementIndex;
    vec3d delta = m_nextMovement[1 - m_movementIndex];
    m_nextMovement[1 - m_movementIndex] = { 0, 0, 0 };
    return delta;
}

}
