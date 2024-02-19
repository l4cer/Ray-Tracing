#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <cmath>
#include <limits>
#include <memory>
#include <random>


const double pi = 3.141592653589;


inline double deg2rad(double deg){
    return deg * pi / 180.0;
};

inline double rad2deg(double rad){
    return rad * 180.0 / pi;
};

inline double random_number(){
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
};

inline double random_double(double min, double max){
    return min + (max - min) * random_number();
};

#endif
