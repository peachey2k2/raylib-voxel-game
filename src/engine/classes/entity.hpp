#pragma once

#include "utils/tools.hpp"

namespace wmac {

class Entity {

protected:
    bool m_active = true;
    vec3d m_pos;

public:
    Entity(const bool p_active, const vec3d& p_pos, const u64 p_id, const void* p_data = nullptr);

};
    
}