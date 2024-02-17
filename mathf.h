#pragma once

#include <array>
#include "types.h"

#define PI 3.14159265f

#define DEG_2_RAD (PI/180.f)
#define RAD_2_DEG (180.f/PI)

namespace srend {
    namespace math {

        std::array<float,16> mul4x4(const std::array<float,16> & a, const std::array<float,16> & b);  
        std::array<float,9> mul3x3(const std::array<float,9> & a, const std::array<float,9> & b);

        float2 mul(const std::array<float,9> & a, const float2 &b);
        float2 mul(const float2 &,const std::array<float,9> &);

        float determinant2x2(const std::array<float,4>&);
        float determinant3x3(const std::array<float,9>&);

        std::array<float,9> cofactor3x3(const std::array<float,9>&);
        std::array<float,4> minor3x3(const std::array<float,9>&, int, int);

        std::array<float,9> transpose3x3(const std::array<float,9>&);
        std::array<float,9> inverse3x3(const std::array<float,9>&);

        float lerp(float a, float b, float t);
        int lerp(int a, int b, float t);

        float clamp(float a, float b, float v);
        int clamp(int a, int b, int v);
    };
};