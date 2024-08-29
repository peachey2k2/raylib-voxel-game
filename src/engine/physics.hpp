#pragma once

#include "utils/tools.hpp"

namespace wmac::physics {

#ifdef PHYSICS_CPP



#endif

void init();
void update();

void accelerate();
void move();
void aabbCollision();
void worldCollision();

}