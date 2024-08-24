#pragma once

#include "./utils.hpp"

namespace wmac::tools {

// convenient debug print, prints all arguments separated by spaces
template<typename T, typename... Args>
void say(const T& first, const Args&... args) {
    std::cout << first;
    if constexpr (sizeof...(args) > 0) {
        std::cout << ' ';
        say(args...);
    } else {
        std::cout << '\n';
    }
}

// simple benchmarker. always create with `new`. deleted automatically
class Benchmark {

private:
    std::string m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

public:
    Benchmark(std::string p_name = "Benchmark") {
        m_start = std::chrono::high_resolution_clock::now();
        m_name = p_name;
    }

    void end() {
        auto end = std::chrono::high_resolution_clock::now();
        f64 duration = std::chrono::duration<f64>(end - m_start).count();
        say("[BENCHMARK]", m_name, "took", duration, "seconds");
        delete this;
    }
};


}