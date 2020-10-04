#pragma once
#include <string>
#include <stdexcept>

class nephroid
{
private:
    double r;
public:
    static constexpr double ABS_ERROR = 0.000000000001;
    nephroid() {
        r = 0;
    }
    nephroid(double a) {
        if (a < 0)
            throw std::exception("Invalid radius\n");
        r = a;
    }
    void radius(double);
    double Get_L() const;
    double Get_S() const;
    double Get_R(double) const;
    double Get_rad() const;
    double Get_x(double) const;
    double Get_y(double) const;
    std::string Get_eq() const;
};


