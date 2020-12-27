#pragma once

#include <iostream>
#include <concepts>
#include <random>
#include <iterator>

template<typename T1, typename T2>
T1 SelectRandomly(T1 start, T1 end, T2& g) {
	std::uniform_int_distribution<size_t> dis(0LLU, std::distance(start, end) - 1LLU);
	std::advance(start, dis(g));
	return start;
}

template<typename T>
T SelectRandomly(T start, T end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return SelectRandomly(start, end, gen);
}

template<typename T>
auto SelectRandomly(const T& cont) {
	return SelectRandomly(std::begin(cont), std::end(cont));
}
