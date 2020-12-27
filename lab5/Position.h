#pragma once
#include <vector>

class Position {
private:
	int markers;
	std::vector<int> out;
public:
	Position() : markers{} {}
	Position(int transitions) : markers{}, out(transitions) {}
	Position(std::vector<int>&& out) : markers{}, out(std::move(out)) {}
	auto get_num_of_markers() const { return markers; }
	const auto& get_out() const { return out; }
	void add_markers(const int num) { markers += num; }
	void remove_markers(const int num) { markers -= num; }
	void update_transition(const int idx, const int num) { out.at(idx) += num; }
};
