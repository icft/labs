#include "nephroid.h"
#include <iostream>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <string>


double nephroid::Get_L() const {
    return 24 * r;
}

double nephroid::Get_S() const {
    return 12 * M_PI * r * r;
}

double nephroid::Get_R(double t) const {
    return 4 * r * sin(t * M_PI / 180);
}

double nephroid::Ger_rad() const
{
    return r;
}

double nephroid::Get_x(double t) const {
    return 6 * r * cos(t * M_PI / 180) - 4 * r * pow(cos(t * M_PI / 180), 3);
}

double nephroid::Get_y(double t) const {
    return 4 * r * pow(sin(t * M_PI / 180), 3);
}

std::string nephroid::Get_eq() const {
    std::string x = "(x^2++y^2+" + std::to_string(4 * r * r) + ")^3 = " + std::to_string(180 * pow(r, 4)) + "y^2";
    return x;
}

void nephroid::radius(double rad) {
    r = rad;
}
