#pragma once

#include "utils/tools.hpp"

#include "classes/entity.hpp"

namespace wmac::entities {

#ifdef ENTITIES_CPP

#endif

inline std::vector<Entity*> m_activeEntities;
inline std::vector<Entity*> m_inactiveEntities;

void init();
void add(InitEntityInfo &p_entity);
// void spawnRandomly(EntityID &p_id);

}