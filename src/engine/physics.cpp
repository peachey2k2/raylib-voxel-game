#define PHYSICS_CPP
#include "./physics.hpp"

#include "engine/entities.hpp"

namespace wmac::physics {

void init() {
}

void update() {
    accelerate();
    move();
    worldCollision();
    aabbCollision();
}

void accelerate() {

}

void move() {
    #pragma omp parallel for
    for (auto& entity : entities::m_activeEntities) {
        entity->applyMovement();
    }
}

void worldCollision() {
    
}

void aabbCollision() {

}

}


