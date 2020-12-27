#pragma once
#include "Marker.h"
#include "Position.h"
#include "Transition.h"
#include <atomic>
#include <mutex>
#include <iostream>
#include <concepts>
#include <random>
#include <iterator>

class PetriNet {
private:
	std::vector<Marker> markers;
	std::vector<Position> positions;
	std::vector<Transition> transitions;
	std::vector<int> queue;
	std::mutex markers_lock;
	std::mutex queue_lock;
	std::atomic_bool all_blocked;
	std::chrono::time_point<std::chrono::system_clock> time;
	std::chrono::milliseconds duration;
	void generate_marker_duration(Marker& marker);
	void generate_markers_duration();
	void print_condition();
	std::vector<int> get_ready_input_markers(const int transitionIdx);
	void delete_markers(std::vector<int>& m, std::ofstream& ofstr);
	void move_markers(const int transitionIdx, std::ofstream& ofstr);
	void update_markers();
	void update_transitions(std::ofstream& ofstr);
	void perform_transitions(std::ofstream& ofstr);
public:
	PetriNet(int positions, int transitions) :
		positions(positions, Position(transitions)),
		transitions(transitions, Transition(positions)),
		duration{}
	{}
	auto get_time() const { return time; }
	auto get_duration() const { return duration; }
	void run(std::chrono::seconds duration, std::ofstream& f);
	void add_markers(const int positionIdx, const int num) {
		positions.at(positionIdx).add_markers(num);
		for (int i = 0; i < num; ++i)
			markers.emplace_back(positionIdx);
	}
	void add_pt_edge(const int positionIdx, const int transitionIdx, const int num) {
		positions.at(positionIdx).update_transition(transitionIdx, num);
		transitions.at(transitionIdx).update_input(positionIdx, num);
	}
	void add_tp_edge(const int transitionIdx, const int positionIdx, const int num) { transitions.at(transitionIdx).update_output(positionIdx, num); }
};