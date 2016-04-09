/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#ifndef LEC_ACC_CPP_INTERVAL_H
#define LEC_ACC_CPP_INTERVAL_H

#include <random>

class Interval {
private:
    std::random_device r;
    std::default_random_engine e1;
    std::uniform_real_distribution<double> uniform_dist;
    std::normal_distribution<double> normal_dist;
    double min, max;

    template <typename T>
    T clip(const T& n, const T& lower, const T& upper) {
        return std::max(lower, std::min(n, upper));
    }

public:
    Interval(double min, double max) : min(min), max(max) {
        e1 = std::default_random_engine(r());
        uniform_dist = std::uniform_real_distribution<double>(min, max);
        normal_dist = std::normal_distribution<double>((min + max) / 2, (max - min) / 4);
    }

    double uniform() {
        return uniform_dist(e1);
    }

    double normal() {
        return clip(normal_dist(e1), min, max);
    }
};

#endif
