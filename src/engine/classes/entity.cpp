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

}