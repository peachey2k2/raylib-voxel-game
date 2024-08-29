#define TICKS_CPP
#include "./ticks.hpp"

#include "utils/tools.hpp"
#include "./core.hpp"
#include "./render.hpp"
#include "./physics.hpp"

namespace wmac::ticks {

const u32 TICKS_PER_SECOND = 20;
const f64 TICK_RATE = 1.0 / TICKS_PER_SECOND;

void checkLoop() {
    physics::init();
    while (core::worldShouldTick()) {
        m_tickDesync += GetFrameTime();

        if (m_tickDesync >= TICK_RATE) {
            m_tickDesync -= TICK_RATE;
            update();
            physics::update();
        }
        i64 sleepTime = ((TICK_RATE - m_tickDesync)*1000) > 10 ? (TICK_RATE - m_tickDesync - 5)*1000 - 5 : 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

void update() {
    for (auto callback : m_tickCallbacks) {
        callback();
    }
}

void addCallback(void (*p_callback)()) {
    m_tickCallbacks.push_back(p_callback);
}

void removeCallback(void (*p_callback)()) {
    for (auto it = m_tickCallbacks.begin(); it != m_tickCallbacks.end(); it++) {
        if (*it == p_callback) {
            m_tickCallbacks.erase(it);
            return;
        }
    }
    tools::say("--Callback not found");
}

};