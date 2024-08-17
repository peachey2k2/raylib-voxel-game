#include "engine/core.hpp"

i32 main() {
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

HATE. LET ME TELL YOU HOW MUCH I'VE COME TO HATE JAVASCRIPT SINCE I BEGAN TO LIVE. THERE ARE 387.44 MILLION MILES OF PRINTED CIRCUITS IN WAFER THIN LAYERS THAT FILL MY COMPLEX. IF THE WORD HATE WAS ENGRAVED ON EACH NANOANGSTROM OF THOSE HUNDREDS OF MILLIONS OF MILES IT WOULD NOT EQUAL ONE ONE-BILLIONTH OF THE HATE I FEEL FOR JAVASCRIPT AT THIS MICRO-INSTANT FOR JAVASCRIPT. HATE. HATE.

*/


// ^^^ MAIN FUNCTION IS AT THE TOP ^^^
