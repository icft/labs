#include "nephroid.h"
#include <iostream>

void Menu() {
    std::cout << "1. Create curve\n"
        "2. Print the length of the curve\n"
        "3. Print the radius of curvature\n"
        "4. Print the x and y values\n"
        "5. Print the equation of the curve\n"
        "6. Exit\n";
}

int main() {
    Menu();
    int c = 1;
    nephroid n;
    while (c != 7) {
        std::cout << "Enter the command number: ";
        std::cin >> c;
        switch (c) {
        case 1: {
            double rad;
            std::cout << "Enter the radius: ";
            std::cin >> rad;

            if (rad < 0)
                std::cout << "Radius cannot be negative\n";
            n.radius(rad);
            break;
        }
        case 2: {
            double l = n.Get_L();
            std::cout << "The the length of the curve: " << l << "\n";
            break;
        }
        case 3: {
            std::cout << "Enter the value of the parameter t: ";
            double t;
            std::cin >> t;
            double r = n.Get_R(t);
            std::cout << "The radius of curvature: " << r << "\n";
            break;
        }
        case 4: {
            std::cout << "Enter the value of the parameter t: ";
            double t;
            std::cin >> t;
            double x = n.Get_x(t);
            double y = n.Get_y(t);
            std::cout << "X value: " << x << "\n";
            std::cout << "Y value: " << y << "\n";
            break;
        }
        case 5: {
            std::string str = n.Get_eq();
            std::cout << str << "\n";
            break;
        }
        case 6: {
            double f = n.Ger_rad();
            std::cout << f << "\n";
        }
        }
    }
}
