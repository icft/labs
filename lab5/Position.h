#pragma once
#include <vector>

class Position {
private:
	size_t markers;
	std::vector<size_t> out;
public:
	Position() : markers{} {} 
	Position(const size_t transitions) : markers{}, out(transitions) {}
	Position(std::vector<size_t>&& out) : markers{}, out(std::move(out)) {}
	constexpr auto get_num() const { return markers; }
	const auto& get_out() const { return out; }
	void add(const size_t num) { markers += num; }
	void remove(const size_t num) { markers -= num; }
	void update(const size_t ind, const size_t num) { out.at(ind) += num; }
};
