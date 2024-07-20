#pragma once

#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <iostream>

#ifndef u8
typedef uint8_t u8;
#endif

#ifndef u16
typedef uint16_t u16;
#endif

#ifndef u32
typedef uint32_t u32;
#endif

#ifndef u64
typedef uint64_t u64;
#endif

#ifndef i8
typedef int8_t i8;
#endif

#ifndef i16
typedef int16_t i16;
#endif

#ifndef i32
typedef int32_t i32;
#endif

#ifndef i64
typedef int64_t i64;
#endif

#ifndef f32
typedef float f32;
#endif

#ifndef f64
typedef double f64;
#endif

#ifndef size_t
typedef std::size_t size_t;
#endif

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;
typedef Matrix mat4;

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



