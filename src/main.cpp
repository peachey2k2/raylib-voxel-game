#include "engine/core.hpp"

int main() {
    try {
        wmac::core::run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

/*

Fuck it, I'll just type whatever i have in mind into here. If I encounter something weird, intertesting, important, sometimes even unrelated stuff in here.

PLEASE PLEASE PLEASE use m_ for member variables, I don't want to spend another whole day losing my mind because of a scope (skill) issue.

I used greedy meshing for performance, it's decent for now, maybe that'll change later on when I implement all the dynamic texture logic.

Appaerently some genius thought not filtering out spaces at the end of the line in makefiles was a good idea.

*/


// ^^^ MAIN FUNCTION IS AT THE TOP ^^^
