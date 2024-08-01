#pragma once

#include "misc/utils.hpp"

namespace wmac {

}

namespace wmac::ticks {

extern const f64 TICK_RATE; 

#ifdef TICKS_CPP

f64 tickDesync = 0.0;
std::vector<void (*)()> tickCallbacks;

#endif

void check();
void update();

void addCallback(void (*p_callback)());
void removeCallback(void (*p_callback)());
   
}