#define PHYSICS_CPP
#include "./physics.hpp"

#include "./entities.hpp"
#include "./world.hpp"

namespace wmac::physics {

void init() {
}

void update() {
    accelerate();
    move();
}

void accelerate() {

}

void move() {
    #pragma omp parallel for
    for (auto& entity : entities::m_activeEntities) {
        vec3d deltaMovement = entity->resetMovementDelta();
        vec3d curPos = entity->getPos();
        vec3d nextPos = curPos + deltaMovement;
        Range<vec3d> nextCollision = entity->getCollisionBox() + entity->getPos() + deltaMovement;
        
        Range<vec3d> extremes = world::findExtremes(nextCollision);
    }
}

}


