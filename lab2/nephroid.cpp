#include "nephroid.h"
#include <iostream>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>

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
    std::stringstream s;
    double a = 4 * r * r, b = 108 * pow(r,4);
    s << "(x^2++y^2+" << a << ")^3=" << b << "*y^2";
    std::string str;
    s >> str;
    return str;
}

void nephroid::radius(double rad) {
    r = rad;
}
