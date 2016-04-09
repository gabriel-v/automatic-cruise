/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#include "Neighbours.h"


Neighbours::Neighbours(Target *front, Target *back) {
    this->back = back;
    this->front = front;
}

Neighbours Neighbours::withLeft(Target *front, Target *back) {
    frontLeft = front;
    backLeft = back;
    return *this;
}

Neighbours Neighbours::withRight(Target *front, Target *back) {
    frontRight = front;
    backRight = back;
    return *this;
}

