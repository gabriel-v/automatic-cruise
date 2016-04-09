/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#ifndef LEC_ACC_CPP_RANDOMVEHICLE_H
#define LEC_ACC_CPP_RANDOMVEHICLE_H

#include "Vehicle.h"

class RandomVehicle: public Vehicle {
public:

    virtual void think(const Neighbours &neighbours, double dt) override;

    RandomVehicle(double x, double v);



};


#endif
