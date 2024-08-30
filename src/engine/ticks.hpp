#pragma once

#include "utils/tools.hpp"

namespace wmac {

}

namespace wmac::ticks {

extern const f64 TICK_RATE; 

#ifdef TICKS_CPP

f64 m_tickDesync = 0.0;
std::vector<void (*)()> m_tickCallbacks;

#endif

void checkLoop();
void update();

void addCallback(void (*p_callback)());
void removeCallback(void (*p_callback)());
   
}