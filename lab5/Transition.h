#pragma once
#include <vector>

class Transition {
private:
	std::vector<int> inp;
	std::vector<int> out;
	void update(std::vector<int>& vec, const int idx, const int num) { vec[idx] += num; }
public:
	Transition() = default;
	Transition(int positions) : inp(positions), out(positions) {}
	Transition(std::vector<int>&& inputs, std::vector<int>&& outputs) : inp(std::move(inputs)), out(std::move(outputs)) {}
	const auto& get_inputs() const { return inp; }
	const auto& get_outputs() const { return out; }
	void update_input(const int idx, const int num) { update(inp, idx, num); }
	void update_output(const int idx, const int num) { update(out, idx, num); }
};
