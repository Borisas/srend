#pragma once

#include <random>

namespace srend {
    class Random {
    public:
        Random(long seed);
        Random();
        ~Random();
    
        int int32(int min, int max);
        float float32(float min, float max);
        float one();

    private:
        std::mt19937 _rand;
        std::uniform_real_distribution<double> _distribution;
    };
}