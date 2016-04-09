/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#ifndef LEC_ACC_CPP_NEIGHBOURS_H
#define LEC_ACC_CPP_NEIGHBOURS_H


#include "Vehicle.h"
#include "Target.h"

class Neighbours {
    public:
    Target *front, *back, *frontLeft, *frontRight, *backLeft, *backRight;

    Neighbours(Target *front, Target *back);
    Neighbours withLeft(Target *front, Target *back);
    Neighbours withRight(Target *front, Target *back);
};


#endif
