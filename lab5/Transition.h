#pragma once
#include <vector>

class Transition {
private:
	std::vector<size_t> inp;
	std::vector<size_t> out;
	void update(std::vector<size_t>& vec, const size_t ind, const size_t num) { vec.at(ind) += num; }
public:
	Transition() = default;
	Transition(const std::size_t positions) : inp(positions), out(positions) {}
	Transition(std::vector<size_t>&& inputs, std::vector<size_t>&& outputs) : inp(std::move(inputs)), out(std::move(outputs)) {}
	const auto& get_inp() const noexcept { return inp; }
	const auto& get_out() const noexcept { return out; }
	void update_inp(const size_t ind, const size_t num) { update(inp, ind, num); }
	void update_out(const size_t ind, const size_t num) { update(out, ind, num); }
};
