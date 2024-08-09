#include "core/core.hpp"

int main() {
    wmac::Core core;
    try {
        core.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

/*

Fuck it, I'll just type whatever i have in mind into here. If I encounter something weird, intertesting, important, sometimes even unrelated stuff in here.

PLEASE PLEASE PLEASE use m_ for member variables, I don't want to spend another whole day losing my mind because of a scope (skill) issue.

*/
