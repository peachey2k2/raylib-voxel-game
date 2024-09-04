#define TICKS_CPP
#include "./ticks.hpp"

#include "utils/tools.hpp"
#include "./core.hpp"
#include "./render.hpp"
#include "./physics.hpp"

namespace wmac::ticks {

// pretty much everything is tied to tick speed.
const u32 TICKS_PER_SECOND = 20;
const f64 TICK_RATE = 1.0 / TICKS_PER_SECOND;

// subticks should be used in moderation, only for when precision is needed. (i.e. player movement)
const u32 SUBTICKS_PER_TICK = 3;
const f64 SUBTICK_RATE = TICK_RATE / SUBTICKS_PER_TICK;

void checkLoop() {
    u32 subtick = 0;
    physics::init();
    while (core::worldShouldTick()) {
        m_tickDesync += GetFrameTime();

        if (m_tickDesync >= SUBTICK_RATE) {
            m_tickDesync -= SUBTICK_RATE;
            
            subtickUpdate();

            subtick++;
            if (subtick == SUBTICKS_PER_TICK) {
                subtick = 0;
                update();
                physics::update();
            }
        }
        i64 sleepTime = ((SUBTICK_RATE - m_tickDesync)*1000) > 10 ? (SUBTICK_RATE - m_tickDesync - 5)*1000 - 5 : 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

void update() {
    for (auto& [callback, interval, counter] : m_tickCallbacks) {
        counter--;
        if (counter == 0) {
            callback();
            counter = interval;
        }
    }
}

void addCallback(void (*p_callback)(), u32 p_interval) {
    m_tickCallbacks.push_back({p_callback, p_interval, p_interval});
}

void removeCallback(void (*p_callback)()) {
    for (auto it = m_tickCallbacks.begin(); it != m_tickCallbacks.end(); it++) {
        if (it->callback == p_callback) {
            m_tickCallbacks.erase(it);
            return;
        }
    }
    tools::say("--Callback not found");
}

void subtickUpdate() {
    for (auto& [callback, interval, counter] : m_subtickCallbacks) {
        counter--;
        if (counter == 0) {
            callback();
            counter = interval;
        }
    }
}

void subtickAddCallback(void (*p_callback)(), u32 p_interval) {
    m_subtickCallbacks.push_back({p_callback, p_interval, p_interval});
}

void subtickRemoveCallback(void (*p_callback)()) {
    for (auto it = m_subtickCallbacks.begin(); it != m_subtickCallbacks.end(); it++) {
        if (it->callback == p_callback) {
            m_subtickCallbacks.erase(it);
            return;
        }
    }
    tools::say("--Callback not found");
}

};