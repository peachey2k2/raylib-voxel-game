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
    for (auto& entity : entities::m_activeEntities) {
        
    }
}

void worldCollision() {
    
}

void aabbCollision() {

}

}


