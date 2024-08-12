#pragma once

#include <cstdlib>
#include <cstdint>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "raylib.hpp"
#include "./types.hpp"

#define wmac WorldMachine

namespace wmac {

#define ASSERT(x, msg) if (!(x)) { throw std::runtime_error(msg); }

// mmmmm gotta save those keystrokes
#define scast static_cast
#define dcast dynamic_cast
#define ccast const_cast
#define rcast reinterpret_cast

// convenient debug print
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

};

inline const mat4 IDENTITY_MATRIX = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
}; 

constexpr vec2 operator+(const vec2& a, const vec2& b) {
    return { a.x + b.x, a.y + b.y };
}

constexpr vec2 operator-(const vec2& a, const vec2& b) {
    return { a.x - b.x, a.y - b.y };
}

constexpr vec2 operator*(const vec2& a, const f32& b) {
    return { a.x * b, a.y * b };
}

constexpr vec2 operator/(const vec2& a, const f32& b) {
    return { a.x / b, a.y / b };
}

constexpr vec3 operator+(const vec3& a, const vec3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr vec3 operator-(const vec3& a, const vec3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr vec3 operator*(const vec3& a, const f32& b) {
    return { a.x * b, a.y * b, a.z * b };
}

constexpr vec3 operator/(const vec3& a, const f32& b) {
    return { a.x / b, a.y / b, a.z / b };
}

constexpr vec4 operator+(const vec4& a, const vec4& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

constexpr vec4 operator-(const vec4& a, const vec4& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

constexpr vec4 operator*(const vec4& a, const f32& b) {
    return { a.x * b, a.y * b, a.z * b, a.w * b };
}

constexpr vec4 operator/(const vec4& a, const f32& b) {
    return { a.x / b, a.y / b, a.z / b, a.w / b };
}

constexpr mat4 operator*(const mat4& a, const mat4& b) {
    return {
        a.m0 * b.m0 + a.m1 * b.m4 + a.m2 * b.m8 + a.m3 * b.m12,
        a.m0 * b.m1 + a.m1 * b.m5 + a.m2 * b.m9 + a.m3 * b.m13,
        a.m0 * b.m2 + a.m1 * b.m6 + a.m2 * b.m10 + a.m3 * b.m14,
        a.m0 * b.m3 + a.m1 * b.m7 + a.m2 * b.m11 + a.m3 * b.m15,
        a.m4 * b.m0 + a.m5 * b.m4 + a.m6 * b.m8 + a.m7 * b.m12,
        a.m4 * b.m1 + a.m5 * b.m5 + a.m6 * b.m9 + a.m7 * b.m13,
        a.m4 * b.m2 + a.m5 * b.m6 + a.m6 * b.m10 + a.m7 * b.m14,
        a.m4 * b.m3 + a.m5 * b.m7 + a.m6 * b.m11 + a.m7 * b.m15,
        a.m8 * b.m0 + a.m9 * b.m4 + a.m10 * b.m8 + a.m11 * b.m12,
        a.m8 * b.m1 + a.m9 * b.m5 + a.m10 * b.m9 + a.m11 * b.m13,
        a.m8 * b.m2 + a.m9 * b.m6 + a.m10 * b.m10 + a.m11 * b.m14,
        a.m8 * b.m3 + a.m9 * b.m7 + a.m10 * b.m11 + a.m11 * b.m15,
        a.m12 * b.m0 + a.m13 * b.m4 + a.m14 * b.m8 + a.m15 * b.m12,
        a.m12 * b.m1 + a.m13 * b.m5 + a.m14 * b.m9 + a.m15 * b.m13,
        a.m12 * b.m2 + a.m13 * b.m6 + a.m14 * b.m10 + a.m15 * b.m14,
        a.m12 * b.m3 + a.m13 * b.m7 + a.m14 * b.m11 + a.m15 * b.m15
    };
}

constexpr vec3 VEC3(const vec2& a, f32 w) {
    return { a.x, a.y, w };
}

constexpr vec4 VEC4(const vec3& a, f32 w) {
    return { a.x, a.y, a.z, w };
}

constexpr vec2i operator+(const vec2i& a, const vec2i& b) {
    return { a.x + b.x, a.y + b.y };
}

constexpr vec2i operator-(const vec2i& a, const vec2i& b) {
    return { a.x - b.x, a.y - b.y };
}

constexpr vec2i operator*(const vec2i& a, const i32& b) {
    return { a.x * b, a.y * b };
}

constexpr vec2i operator/(const vec2i& a, const i32& b) {
    return { a.x / b, a.y / b };
}

constexpr bool operator==(const vec2i& a, const vec2i& b) {
    return (a.x == b.x) && (a.y == b.y);
}

constexpr vec3i operator+(const vec3i& a, const vec3i& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr vec3i operator-(const vec3i& a, const vec3i& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr vec3i operator*(const vec3i& a, const i32& b) {
    return { a.x * b, a.y * b, a.z * b };
}

constexpr vec3i operator/(const vec3i& a, const i32& b) {
    return { a.x / b, a.y / b, a.z / b };
}

constexpr bool operator==(const vec3i& a, const vec3i& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

constexpr vec4i operator+(const vec4i& a, const vec4i& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

constexpr vec4i operator-(const vec4i& a, const vec4i& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

constexpr vec4i operator*(const vec4i& a, const i32& b) {
    return { a.x * b, a.y * b, a.z * b, a.w * b };
}

constexpr vec4i operator/(const vec4i& a, const i32& b) {
    return { a.x / b, a.y / b, a.z / b, a.w / b };
}

constexpr bool operator==(const vec4i& a, const vec4i& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline i32 pow(i32 base, i32 exp) {
    i32 result = 1;
    for (i32 i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

namespace std { 
    // hash function for vec3i, necessary for unordered_map
    template<> struct hash<vec3i> {
        size_t operator()(const vec3i& vec) const noexcept {
            size_t h1 = ::std::hash<int>{}(vec.x);
            size_t h2 = ::std::hash<int>{}(vec.y);
            size_t h3 = ::std::hash<int>{}(vec.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

