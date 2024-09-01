#include "./entity.hpp"

#include "engine/entities.hpp"

namespace wmac {

Entity::Entity(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data) {
    m_active = p_active;
    m_pos = p_pos;
    m_id = p_id;
    
    (m_active ? entities::m_activeEntities : entities::m_inactiveEntities).push_back(this);
    // TODO: implement

}

void Entity::updateRotation(const vec3 p_dir) {
    m_dir = p_dir;
}

void Entity::updateMovementDelta(const vec3d p_deltaMovement) {
    m_movementMutex.lock();
    m_nextMovement += p_deltaMovement;
    m_movementMutex.unlock();
}

vec3d Entity::applyMovement() {
    m_movementMutex.lock();
    m_nextPos += m_nextMovement;
    m_nextMovement = { 0, 0, 0 };
    m_movementMutex.unlock();
}

}