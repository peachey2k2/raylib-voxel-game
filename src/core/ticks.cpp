#define TICKS_CPP
#include "core/ticks.hpp"

namespace wmac::ticks {

const f64 TICK_RATE = 0.05;

void check() {
    tickDesync += raylib::GetFrameTime();
    if (tickDesync >= TICK_RATE) {
        tickDesync -= TICK_RATE;
        update(); // TODO: move this to a separate thread
    }
}

void update() {
    for (auto callback : tickCallbacks) {
        callback();
    }
}

void addCallback(void (*p_callback)()) {
    tickCallbacks.push_back(p_callback);
}

void removeCallback(void (*p_callback)()) {
    for (auto it = tickCallbacks.begin(); it != tickCallbacks.end(); it++) {
        if (*it == p_callback) {
            tickCallbacks.erase(it);
            return;
        }
    }
    say("--Callback not found");
}

};