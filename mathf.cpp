#include "mathf.h"

std::array<float,16> srend::math::mul4x4(const std::array<float,16> & a, const std::array<float,16> & b) {

    return std::array<float,16>({
        a[0] * b[0] + a[1] * b[0+1*4] + a[2] * b[0+2*4] + a[3] * b[0+3*4],
        a[0] * b[1] + a[1] * b[1+1*4] + a[2] * b[1+2*4] + a[3] * b[1+3*4],
        a[0] * b[2] + a[1] * b[2+1*4] + a[2] * b[2+2*4] + a[3] * b[2+3*4],
        a[0] * b[3] + a[1] * b[3+1*4] + a[2] * b[3+2*4] + a[3] * b[3+3*4],

        a[0+4*1] * b[0] + a[1+4*1] * b[0+1*4] + a[2+4*1] * b[0+2*4] + a[3+4*1] * b[0+3*4],
        a[0+4*1] * b[1] + a[1+4*1] * b[1+1*4] + a[2+4*1] * b[1+2*4] + a[3+4*1] * b[1+3*4],
        a[0+4*1] * b[2] + a[1+4*1] * b[2+1*4] + a[2+4*1] * b[2+2*4] + a[3+4*1] * b[2+3*4],
        a[0+4*1] * b[3] + a[1+4*1] * b[3+1*4] + a[2+4*1] * b[3+2*4] + a[3+4*1] * b[3+3*4],

        a[0+4*2] * b[0] + a[1+4*2] * b[0+1*4] + a[2+4*2] * b[0+2*4] + a[3+4*2] * b[0+3*4],
        a[0+4*2] * b[1] + a[1+4*2] * b[1+1*4] + a[2+4*2] * b[1+2*4] + a[3+4*2] * b[1+3*4],
        a[0+4*2] * b[2] + a[1+4*2] * b[2+1*4] + a[2+4*2] * b[2+2*4] + a[3+4*2] * b[2+3*4],
        a[0+4*2] * b[3] + a[1+4*2] * b[3+1*4] + a[2+4*2] * b[3+2*4] + a[3+4*2] * b[3+3*4],

        a[0+4*3] * b[0] + a[1+4*3] * b[0+1*4] + a[2+4*3] * b[0+2*4] + a[3+4*3] * b[0+3*4],
        a[0+4*3] * b[1] + a[1+4*3] * b[1+1*4] + a[2+4*3] * b[1+2*4] + a[3+4*3] * b[1+3*4],
        a[0+4*3] * b[2] + a[1+4*3] * b[2+1*4] + a[2+4*3] * b[2+2*4] + a[3+4*3] * b[2+3*4],
        a[0+4*3] * b[3] + a[1+4*3] * b[3+1*4] + a[2+4*3] * b[3+2*4] + a[3+4*3] * b[3+3*4],
    });
}   

std::array<float,9> srend::math::mul3x3(const std::array<float,9> & a, const std::array<float,9> & b) {

    return std::array<float,9>({
        a[0] * b[0] + a[1] * b[0+1*3] + a[2] * b[0+2*3],
        a[0] * b[1] + a[1] * b[1+1*3] + a[2] * b[1+2*3],
        a[0] * b[2] + a[1] * b[2+1*3] + a[2] * b[2+2*3],

        a[0+1*3] * b[0] + a[1+1*3] * b[0+1*3] + a[2+1*3] * b[0+2*3],
        a[0+1*3] * b[1] + a[1+1*3] * b[1+1*3] + a[2+1*3] * b[1+2*3],
        a[0+1*3] * b[2] + a[1+1*3] * b[2+1*3] + a[2+1*3] * b[2+2*3],

        a[0+2*3] * b[0] + a[1+2*3] * b[0+1*3] + a[2+2*3] * b[0+2*3],
        a[0+2*3] * b[1] + a[1+2*3] * b[1+1*3] + a[2+2*3] * b[1+2*3],
        a[0+2*3] * b[2] + a[1+2*3] * b[2+1*3] + a[2+2*3] * b[2+2*3]
    });
}

srend::float2 srend::math::mul(const std::array<float, 9>& a, const srend::float2& b) {
    auto ret = float2();
    ret.x = a[0] * b.x + a[1] * b.y + a[2] * 1.0f;
    ret.y = a[0+1*3] * b.x + a[1+1*3] * b.y + a[2+1*3] * 1.0f;
    return ret;
}

srend::float2 srend::math::mul(const float2& b, const std::array<float, 9>& a) {
    auto ret = srend::float2();
    ret.y = a[0] * b.x + a[1] * b.y + a[2] * 1.0f;
    ret.x = a[0+1*3] * b.x + a[1+1*3] * b.y + a[2+1*3] * 1.0f;
    return ret;
}

float srend::math::determinant2x2(const std::array<float,4>& m){
    return m[0] * m[3] - m[1] * m[2];
}

float srend::math::determinant3x3(const std::array<float,9>& m){
    float r = 0.0f;
    auto v0 = m[0] * determinant2x2(minor3x3(m,0,0));
    auto v1 = m[1] * determinant2x2(minor3x3(m,1,0));
    auto v2 = m[2] * determinant2x2(minor3x3(m,2,0));
    // r += m[0] * determinant2x2(minor3x3(m,0,0));
    // r += m[1] * determinant2x2(minor3x3(m,1,0));
    // r += m[2] * determinant2x2(minor3x3(m,2,0));
    return v0-v1+v2;
}

std::array<float,9> srend::math::cofactor3x3(const std::array<float,9>& m) {

    std::array<float,9> ret;

    for ( int i = 0; i < 9; i++ ) {
        int y = i / 3;
        int x = i - y * 3;
        ret[i] = pow(-1, x+1 + y+1) * determinant2x2(minor3x3(m,x,y));
    }

    return ret;
}

std::array<float, 4> srend::math::minor3x3(const std::array<float, 9>& m, int i, int j) {

    std::array<float,4> ret;

    int idx = 0;
    for ( int k = 0; k < 9; k++ ) {
        int y = k / 3;
        int x = k - y * 3;

        if ( x == i ) continue;
        if ( y == j ) continue;

        ret[idx] = m[k];
        idx++;
    }

    return ret;
}

std::array<float,9> srend::math::transpose3x3(const std::array<float,9>& m) {
    return {
        m[3*0 + 0], m[3*1 + 0], m[3*2 + 0],
        m[3*0 + 1], m[3*1 + 1], m[3*2 + 1],
        m[3*0 + 2], m[3*1 + 2], m[3*2 + 2]
    };
}

std::array<float,9> srend::math::inverse3x3(const std::array<float,9>& m) {

    auto adjoint = transpose3x3(cofactor3x3(m));
    float d = determinant3x3(m);

    for ( int i = 0; i < adjoint.size(); i++ ) {
        adjoint[i] = adjoint[i] / d;
    }
    return adjoint;
}

float srend::math::lerp(float a, float b, float t) {
    return a + (b-a)*t;
}

int srend::math::lerp(int a, int b, float t) {
    return a + (b-a)*t;
}

float srend::math::clamp(float a, float b, float v) {
  if ( v < a ) return a;
  if ( v > b ) return b;
  return v;
}

int srend::math::clamp(int a, int b, int v) {
    if ( v < a ) return a;
    if ( v > b ) return b;
    return v;
}
