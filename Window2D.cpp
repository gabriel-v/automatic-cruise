/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/10/16
 */

#include "Window2D.h"


void Window2D::drawRect(double left, double right, double bottom, double top) {
    glBegin(GL_POLYGON);
    glVertex2d(left, top);
    glVertex2d(left, bottom);
    glVertex2d(right, bottom);
    glVertex2d(right, top);
    glEnd();
};


std::pair<double, double> Window2D::roadToScreen(double x, double lane) {
    double centerX = highway.prefferredVehicle->getX();
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);

    return std::make_pair((x - centerX) * ratio, LANE_WIDTH * ratio * (lane - (highway.lanes.size() - 1.0) / 2));
}

void Window2D::drawVehicle(const Vehicle &v, double lane) {
    std::pair<double, double> center = roadToScreen(v.getX(), lane);
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);
    glColor3d(v.getR(), v.getG(), v.getB());
    drawRect(center.first - ratio * v.getLength(), center.first + ratio * v.getLength() / 2,
             center.second - ratio * v.getWidth() / 2, center.second + ratio * v.getWidth() / 2);
}

void Window2D::drawDash(double xMeters, double lane) {
    xMeters = int((xMeters) / 16) * 16;
    std::pair<double, double> center = roadToScreen(xMeters - lane * lane * 3, lane);
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);

    double step = ratio * 8;

    for(double x = center.first + maxLeft; x < maxRight; x += step) {
        drawRect(x, x + step / 2, center.second - 0.03, center.second + 0.03);
    }
}

void Window2D::draw() {

    glColor3f(0.1, 0.2, 0.3);
    drawRect(maxLeft, maxRight, -1, 1);

    glColor3f(0.9, 0.9, 0.9);
    drawRect(maxLeft, maxRight, 0.99, 0.93);
    drawRect(maxLeft, maxRight, -0.99, -0.93);

    for(int i = 0; i < highway.lanes.size() - 1; i++) {
        drawDash(highway.prefferredVehicle->getX(), i + 0.5);
    }


    glColor3f(0.7, 0.3, 0.1);
    for(int i = 0; i < highway.lanes.size(); i++) {
        for(const Vehicle *v: highway.lanes[i]->vehicles) {
            drawVehicle(*v, i);
        }
    }


}