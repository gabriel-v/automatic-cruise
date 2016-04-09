/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#include "RandomVehicle.h"

Interval intWidth(3.5, 4.3);
Interval intHeight(5.9, 7.6);


RandomVehicle::RandomVehicle(double x, double v): x(x), v(v), a(0), targetSpeed(v) {
    width = intWidth.normal();
    length = intHeight.normal();
}

void RandomVehicle::think(const Neighbours &neighbours, double dt) override {

}

RandomVehicle::~RandomVehicle() {

}




