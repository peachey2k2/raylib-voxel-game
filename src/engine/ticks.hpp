#pragma once

#include "utils/tools.hpp"

namespace wmac {

}

namespace wmac::ticks {

extern const u32 TICKS_PER_SECOND;
extern const f64 TICK_RATE;

extern const u32 SUBTICKS_PER_TICK;
extern const f64 SUBTICK_RATE;

#ifdef TICKS_CPP

typedef struct TickCallback {
    void (*callback)();
    u32 interval;
    u32 counter;
} TickCallback;

f64 m_tickDesync = 0.0;
std::vector<TickCallback> m_tickCallbacks;
std::vector<TickCallback> m_subtickCallbacks;

#endif

void checkLoop();

void update();
void addCallback(void (*p_callback)(), u32 p_interval = 1);
void removeCallback(void (*p_callback)());

void subtickUpdate();
void subtickAddCallback(void (*p_callback)(), u32 p_interval = 1);
void subtickRemoveCallback(void (*p_callback)());

   
}