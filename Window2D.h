/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/10/16
 */

#ifndef LEC_ACC_CPP_WINDOW2D_H
#define LEC_ACC_CPP_WINDOW2D_H


#include "Window.h"

class Window2D : public Window {

    std::pair<double, double> roadToScreen(double x, double lane);

    void drawVehicle(const Vehicle &v, double lane);

    void drawDash(double xMeters, double yScreen);

    void drawRect(double left, double right, double bottom, double top);

public:
    virtual void draw();

    Window2D(Highway& highway): Window(highway) {}

    Window2D(const Window& other): Window(other) {}
};


#endif
