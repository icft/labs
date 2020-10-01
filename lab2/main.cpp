#include "nephroid.h"
#include <iostream>

void Menu() {
    std::cout <<
        "1. Print the length of the curve\n"
        "2. Print the radius of curvature\n"
        "3. Print the x and y values\n"
        "4. Print area bounded curve\n"
        "5. Print the equation of the curve\n"
        "6. Print the radius of curve\n"
        "7. Change the radius of curve\n"
        "8. Exit\n";
}

int main() {
    int c = 1;
    std::cout << "Create curve\n";
    nephroid n;
    double rad;
    while (true) {
        std::cout << "Enter the radius: ";
        std::cin >> rad;
        if (rad < 0) {
            std::cout << "Radius cannot be negative. Try again.\n";
            continue;
        }
        n.radius(rad);
        break;
    }
    std::cout << "Functions:\n";
    Menu();
    while (c != 8) {
        std::cout << "Enter the command number: ";
        std::cin >> c;
        switch (c) {
        case 1: {
            double l = n.Get_L();
            std::cout << "The the length of the curve: " << l << "\n";
            break;
        }
        case 2: {
            std::cout << "Enter the value of the parameter t: ";
            double t;
            std::cin >> t;
            double r = n.Get_R(t);
            std::cout << "The radius of curvature: " << r << "\n";
            break;
        }
        case 3: {
            std::cout << "Enter the value of the parameter t: ";
            double t;
            std::cin >> t;
            double x = n.Get_x(t);
            double y = n.Get_y(t);
            std::cout << "X value: " << x << "\n";
            std::cout << "Y value: " << y << "\n";
            break;
        }
        case 4: {
            double s = n.Get_S();
            std::cout << "Area: " << s << "\n";
        }
        case 5: {
            std::string str = n.Get_eq();
            std::cout << str << "\n";
            break;
        }
        case 6: {
            double f = n.Ger_rad();
            std::cout << "Radius: " << f << "\n";
            break;
        }
        case 7: {
            double rad;
            std::cout << "Enter the new radius: ";
            std::cin >> rad;
            n.radius(rad);
        }
        }
    }
}
