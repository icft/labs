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
	std::vector<size_t> queue;
	std::mutex markers_lock;
	std::mutex queue_lock;
	std::atomic_bool all_blocked;
	std::chrono::time_point<std::chrono::system_clock> time;
	std::chrono::milliseconds duration;
	void generate_mar_dur(Marker&);
	void generate_mar_dur();
	std::vector<size_t> get_ready_inp_mar(const size_t);
	void delete_mar(std::vector<size_t>&, std::ofstream&);
	void move_mar(const size_t, std::ofstream&);
	void update_mar();
	void update_tr(std::ofstream&);
	void perform_tr(std::ofstream&);
public:
	PetriNet(const size_t pos, const size_t tr) : positions(pos, Position(tr)), transitions(tr, Transition(pos)), duration{} {}
	constexpr auto get_time() const { return time; }
	constexpr auto get_duration() const { return duration; }
	void run(std::chrono::seconds, std::ofstream&);
	void add_mar(const size_t pos_ind, const size_t num) {
		positions.at(pos_ind).add(num);
		for (size_t i{}; i < num; ++i)
			markers.emplace_back(pos_ind);
	}
	void add_pt(const size_t pos_ind, const size_t tr_ind, const size_t num) {
		positions.at(pos_ind).update(tr_ind, num);
		transitions.at(tr_ind).update_inp(pos_ind, num);
	}
	void add_tp(const size_t tr_ind, const size_t pos_ind, const size_t num) { 
		transitions.at(tr_ind).update_out(pos_ind, num); 
	}
};
