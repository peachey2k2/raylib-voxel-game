#define TICKS_CPP
#include "./ticks.hpp"

namespace wmac::ticks {

const f64 TICK_RATE = 0.05;

void check() {
    m_tickDesync += raylib::GetFrameTime();
    if (m_tickDesync >= TICK_RATE) {
        m_tickDesync -= TICK_RATE;
        update(); // TODO: move this to a separate thread
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
    say("--Callback not found");
}

};