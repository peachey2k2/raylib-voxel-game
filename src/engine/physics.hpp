#pragma once

#include "utils/tools.hpp"

namespace wmac::physics {

#ifdef PHYSICS_CPP



#endif

void init();
void update();

void accelerate();
void move();
    vec3d collideMove(const vec3d& p_curPos, const vec3d& p_deltaMovement, const Range<vec3d>& p_collider);

}