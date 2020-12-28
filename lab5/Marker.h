#pragma once
#include <vector>
#include <chrono>

class Marker{
private:
	bool blocked;
	size_t id;
	size_t position;
	std::vector<size_t> transitions;
	std::chrono::time_point<std::chrono::system_clock> time;
	std::chrono::milliseconds duration;
	inline static auto ID{ 0LLU };
public:
	Marker(const size_t pos) : blocked{}, id(Marker::ID++), position(pos), duration{} {} 
	constexpr auto get_id() const { return id; } 
	constexpr auto get_position() const { return position; } 
	const auto& get_transitions() const { return transitions; }
	constexpr auto get_time() const  { return time; }
	constexpr auto get_duration() const { return duration; }
	constexpr bool is_blocked() const { return blocked; }
	void set_blocked(const bool value) { blocked = value; }
	void set_position(const size_t pos) { position = pos; }
	void set_timepoint(const std::chrono::time_point<std::chrono::system_clock>& t) { time = t; }
	void set_duration(const std::chrono::milliseconds& dur) { duration = dur; }
	void add_transition(const size_t idx) { transitions.emplace_back(idx); }
	void clear_transitions() { transitions.clear(); };
};
