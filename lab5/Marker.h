#pragma once
#include <vector>
#include <chrono>

class Marker {
private:
	unsigned long long ID = 0LLU;
	bool blocked;
	int id;
	int pos;
	std::vector<int> transitions;
	std::chrono::time_point<std::chrono::system_clock> time;
	std::chrono::milliseconds duration;
public:
	Marker(const int position) : blocked{}, id(Marker::ID++), pos(position), duration{} {}
	int get_id() const { return id; }
	int get_position() const { return pos; }
	const auto& get_transitions() const { return transitions; }
	auto get_time() const  { return time; }
	auto get_duration() const { return duration; }
	bool is_blocked() const { return blocked; }
	void set_blocked(const bool value) { blocked = value; }
	void set_position(const int position) { pos = position; }
	void set_time(const std::chrono::time_point < std::chrono::system_clock>& t) { time = t; }
	void set_duration(const std::chrono::milliseconds& d) { duration = d; }
	void add_transition(const int idx) { transitions.emplace_back(idx); }
	void clear_transitions() { transitions.clear(); };
};
