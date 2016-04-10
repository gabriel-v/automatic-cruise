/*
 *  Copyright (c)  2016, Gabriel Vijiala, Stefan Teodorescu
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors may
 *  be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Neighbours.h"


Neighbours::Neighbours(Target *front, Target *back) {
    this->back = back;
    this->front = front;
    backLeft = backRight = frontLeft = frontRight = nullptr;
}

Neighbours *Neighbours::withLeft(Target *front, Target *back) {
    frontLeft = front;
    backLeft = back;
    return this;
}

Neighbours *Neighbours::withRight(Target *front, Target *back) {
    frontRight = front;
    backRight = back;
    return this;
}

Neighbours::Neighbours(const Neighbours &other){
    if(other.front != nullptr) front = new Target(*other.front);
    if(other.back != nullptr) back = new Target(*other.back);
    if(other.frontLeft != nullptr) frontLeft = new Target(*other.frontLeft);
    if(other.frontRight != nullptr) frontRight = new Target(*other.frontRight);
    if(other.backLeft != nullptr) backLeft = new Target(*other.backLeft);
    if(other.backRight != nullptr) backRight = new Target(*other.backRight);
}

Neighbours::Neighbours() :
        front(nullptr), back(nullptr),
        frontLeft(nullptr), frontRight(nullptr),
        backLeft(nullptr), backRight(nullptr) {

}

Neighbours::~Neighbours() {
    if (front != nullptr) delete front;
    if (back != nullptr) delete back;
    if (frontLeft != nullptr) delete frontLeft;
    if (frontRight != nullptr) delete frontRight;
    if (backLeft != nullptr) delete backLeft;
    if (backRight != nullptr) delete backRight;
}





