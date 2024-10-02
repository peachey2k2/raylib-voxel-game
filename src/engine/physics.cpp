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
        if (deltaMovement == vec3d::zero) continue;

        vec3d curPos = entity->getPos();
        Range<vec3d> collider = entity->getCollisionBox();

        vec3d move = collideMove(curPos, deltaMovement, collider);
        entity->setPos(curPos + move);
    }
}

vec3d collideMove(const vec3d& p_curPos, const vec3d& p_deltaMovement, const Range<vec3d>& p_collider) {
    static const std::array<Axis, 3> order = { AXIS_Y, AXIS_X, AXIS_Z };
    vec3d nextPos;

    for (auto& axis : order) {
        bool sign = p_deltaMovement[axis] > 0;
        f64 hitboxOffset = sign ? p_collider.min[axis] : p_collider.max[axis];
        vec3d testMove = p_curPos;
        testMove[axis] += p_deltaMovement[axis];
        Range<vec3d> col = p_collider + testMove;
        f64 extreme = world::findExtreme(col, axis, sign);
        nextPos[axis] = sign ? 
            std::max(testMove[axis], extreme - hitboxOffset) :
            std::min(testMove[axis], extreme - hitboxOffset);
        nextPos[axis] = std::clamp(
            nextPos[axis],
            std::min(0.0, p_deltaMovement[axis]),
            std::max(0.0, p_deltaMovement[axis])
        );
    }

    tools::say(p_curPos + p_deltaMovement, nextPos);
    return nextPos - p_curPos;
}

}


