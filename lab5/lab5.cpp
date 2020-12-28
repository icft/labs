#include "PetriNet.h"
#include <fstream>
#include <iostream>

void Menu() {
	std::cout << "1. Add marker\n";
	std::cout << "2. Add PT edge\n";
	std::cout << "3. Add TP edge\n";
	std::cout << "4. Start\n";
	std::cout << "5. Exit\n";
}

void add_marker(PetriNet& pt) {
	int ind, num;
	std::cout << "Enter index and number of markers: ";
	std::cin >> ind >> num;
	//pt.add_markers(ind, num);
}

void add_pt(PetriNet& pt) {
	int ind_t, ind_p, num;
	std::cout << "Position index: ";
	std::cin >> ind_p;
	std::cout << "Transition index: ";
	std::cin >> ind_t;
	std::cout << "Number of markers: ";
	std::cin >> num;
	//pt.add_pt_edge(ind_p, ind_t, num);
}

void add_tp(PetriNet& pt) {
	int ind_t, ind_p, num;
	std::cout << "Transition index: ";
	std::cin >> ind_t;
	std::cout << "Position index: ";
	std::cin >> ind_p;
	std::cout << "Number of markers: ";
	std::cin >> num;
	//pt.add_tp_edge(ind_t, ind_p, num);
}

void run(PetriNet& pt) {
	std::ofstream ofstr("output.txt");
	int d;
	std::cout << "Enter duration: ";
	std::cin >> d;
	pt.start(std::chrono::seconds(d), ofstr);
	ofstr.close();
}

int main() {
	std::ifstream ifstr("input.txt");
	std::cin.rdbuf(ifstr.rdbuf());
	PetriNet pt(5, 7);
	pt.add_mar(0, 2);
	pt.add_pt(0, 0, 1);
	pt.add_pt(0, 1, 1);
	pt.add_tp(0, 1, 1);
	pt.add_tp(1, 2, 1);
	pt.add_pt(1, 3, 1);
	pt.add_tp(3, 3, 1);
	pt.add_pt(3, 2, 1);
	pt.add_tp(2, 1, 1);
	pt.add_pt(2, 3, 1);
	pt.add_pt(2, 4, 1);
	pt.add_tp(3, 4, 1);
	pt.add_tp(4, 4, 1);
	pt.add_pt(4, 6, 1);
	pt.add_pt(4, 5, 1);
	pt.add_tp(5, 2, 1);
	std::ofstream ofstr("log.txt");
	pt.start(std::chrono::seconds(5), ofstr);
	ofstr.close();
	/*int positions, transitions;
	std::cout << "Enter number of positions: ";
	std::cin >> positions;
	std::cout << "Enter number of transitions: ";
	std::cin >> transitions;
	PetriNet net(positions, transitions);
	Menu();
	int c = 1;
	while (c != 5) {
		std::cout << "Enter command: ";
		std::cin >> c;
		switch (c) {
		case 1: {
			add_marker(net);
			break;
		}
		case 2: {
			add_pt(net);
			break;
		}
		case 3: {
			add_tp(net);
			break;
		}
		case 4: {
			run(net);
			break;
		}
		}
	}
	return 0;*/
}
