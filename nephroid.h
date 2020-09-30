#pragma once
#include <string>

class nephroid
{
private:
    double r;
public:
    nephroid() {
        r = 0;
    }
    nephroid(double a) {
        r = a;
    }
    void radius(double);
    double Get_L() const;
    double Get_S() const;
    double Get_R(double) const;
    double Ger_rad() const;
    double Get_x(double) const;
    double Get_y(double) const;
    std::string Get_eq() const;
};

