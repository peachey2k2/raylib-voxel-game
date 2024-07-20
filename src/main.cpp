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