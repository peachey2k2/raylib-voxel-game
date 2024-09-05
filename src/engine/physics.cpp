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
        Range<vec3d> collider = entity->getCollisionBox();
        Range<vec3d> nextColPos = collider + entity->getPos() + deltaMovement;

        Range<vec3d> colPlaneY = {
            {curPos.x, nextColPos.min.y, curPos.z},
            {curPos.x, nextColPos.max.y, curPos.z},
        };
        Range<vec3d> colPlaneX = {
            {nextColPos.min.x, curPos.y, curPos.z},
            {nextColPos.max.x, curPos.y, curPos.z},
        };
        Range<vec3d> colPlaneZ = {
            {curPos.x, curPos.y, nextColPos.min.z},
            {curPos.x, curPos.y, nextColPos.max.z},
        };
        
        Range<f64> extY = world::findExtreme(colPlaneY, AXIS_Y) - Range<f64>{collider.min.y, collider.max.y};
        Range<f64> extX = world::findExtreme(colPlaneX, AXIS_X) - Range<f64>{collider.min.x, collider.max.x};
        Range<f64> extZ = world::findExtreme(colPlaneZ, AXIS_Z) - Range<f64>{collider.min.z, collider.max.z};

        nextPos.y = std::clamp(nextPos.y, extY.min, extY.max);
        nextPos.x = std::clamp(nextPos.x, extX.min, extX.max);
        nextPos.z = std::clamp(nextPos.z, extZ.min, extZ.max);

        entity->setPosition(nextPos);
    }
}

}


