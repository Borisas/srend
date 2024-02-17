#include "random.h"
#include "mathf.h"
using namespace srend;

Random::Random(long seed) : _rand(seed), _distribution(0.0, 1.0) {}

Random::Random() : _rand(time(NULL)), _distribution(0.0, 1.0) {}

Random::~Random(){}

int srend::Random::int32(int min, int max) {
    return math::lerp(min,max,one());
}

float srend::Random::float32(float min, float max) {
    return math::lerp(min,max,one());
}

float srend::Random::one() {
    return _distribution(_rand);
}
