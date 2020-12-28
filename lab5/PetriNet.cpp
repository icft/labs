#include "PetriNet.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include <set>
#include <string>
#include <fstream>
#include <iostream> 
#include "Random.h"

auto time_now() {
	return std::chrono::system_clock::now();
}

auto time_pass(const std::chrono::time_point<std::chrono::system_clock>& t) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(time_now() - t).count();
}

template<typename _T>
concept timed_object = requires (_T x) { x.get_time(); x.get_duration(); };

template<timed_object _T>
bool duration_pass(const _T& obj) {
	return time_pass(obj.get_time()) >= obj.get_duration().count();
}

std::string log(const std::chrono::time_point<std::chrono::system_clock>& t, const size_t mar_ind, const std::string s) {
	return "Time: " + std::to_string(time_pass(t)) + "ms; Marker index: " + std::to_string(mar_ind) + "; " + s + "\n";
}

void PetriNet::generate_mar_dur(Marker& marker)  {
	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_int_distribution<size_t> dist(0, 1000);
	marker.set_timepoint(time_now());
	marker.set_duration(std::chrono::milliseconds(dist(mt)));
}

void PetriNet::generate_mar_dur()  {
	std::for_each(markers.begin(), markers.end(), [&](Marker& marker) { generate_mar_dur(marker); });
}

std::vector<size_t> PetriNet::get_ready_inp_mar(const size_t ind) {
	std::vector<size_t> inputMarkers;
	const auto& inputs = transitions.at(ind).get_inp();
	for (size_t i = 0; i < inputs.size(); ++i)
		if (inputs[i]) {
			std::lock_guard<std::mutex> lock(markers_lock);
			std::lock_guard<std::mutex> queue_lock(queue_lock);
			for (const auto& marker : markers)
				if (marker.get_position() == i &&
					std::find_if(queue.begin(), queue.end(),
						[&](const size_t mar_ind) { return markers.at(mar_ind).get_id() == marker.get_id(); }) != queue.end())
					inputMarkers.emplace_back(marker.get_id());
		}
	return inputMarkers;
}

void PetriNet::delete_mar(std::vector<size_t>& mar, std::ofstream& ofstr) {
	std::lock_guard<std::mutex> lock(markers_lock);
	for (const auto& mar_to_remove : mar) {
		auto it = std::find_if(markers.begin(), markers.end(), [&](const Marker& marker) { return marker.get_id() == mar_to_remove; });
		if (it != markers.end()) {
			ofstr << log(time, mar_to_remove, "-");
			positions.at(it->get_position()).remove(1);
			const size_t pos = std::distance(markers.begin(), it);
			queue.erase(std::find(queue.begin(), queue.end(), pos));
			std::for_each(queue.begin(), queue.end(), [&](size_t& m_ind) { if (m_ind > pos) --m_ind; });
			markers.erase(it);
		}
	}
	markers.clear();
}

void PetriNet::move_mar(const size_t tr_ind, std::ofstream& f) {
	auto ready_mar = get_ready_inp_mar(tr_ind);
	const auto& outputs = transitions.at(tr_ind).get_out();
	std::vector<size_t> possible_pos;
	for (size_t i{}; i < outputs.size(); ++i)
		if (outputs[i])
			possible_pos.insert(possible_pos.end(), outputs[i], i);
	if (possible_pos.empty())
		delete_mar(ready_mar, f);
	std::lock_guard<std::mutex> lock(markers_lock);
	while (!possible_pos.empty() && !ready_mar.empty()) {
		const auto output = SelectRandomly(possible_pos);
		auto it = std::find_if(markers.begin(), markers.end(), [&](const Marker& marker) { return marker.get_id() == ready_mar.front(); });
		if (it != markers.end()) {
			const auto position = it->get_position();
			const auto msg = "jump " + std::to_string(position) + " -> " + std::to_string(*output) + " throw this transition: " + std::to_string(tr_ind);
			f << log(time, ready_mar.front(), msg);
			positions.at(position).remove(1);
			it->set_position(*output);
			positions.at(*output).add(1);
			generate_mar_dur(*it);
			{
				std::lock_guard queue_lock(queue_lock);
				queue.erase(std::remove_if(queue.begin(), queue.end(),
					[&](const size_t mar_ind) { return markers.at(mar_ind).get_id() == ready_mar.front(); }), queue.end());
			}
			ready_mar.erase(ready_mar.begin());
			possible_pos.erase(output);
		}
	}
}

void PetriNet::update_mar() {
	while (!all_blocked && !markers.empty() && !duration_pass(*this)) {
		std::lock_guard lock(markers_lock);
		std::lock_guard queue_lock(queue_lock);
		for (size_t i{}; i < markers.size(); ++i) {
			if (std::find(queue.begin(), queue.end(), i) != queue.end())
				continue;
			if (duration_pass(markers.at(i)))
				queue.push_back(i);
		}
	}
}

void PetriNet::update_tr(std::ofstream& f) {
	std::lock_guard lock(markers_lock);
	std::lock_guard queue_lock(queue_lock);
	std::for_each(markers.begin(), markers.end(), [](Marker& marker) { marker.clear_transitions(); });
	for (const auto& mar_ind : queue) {
		auto& marker = markers.at(mar_ind);
		const auto mar_pos = marker.get_position();
		const auto& position = positions.at(mar_pos);
		const auto& out = position.get_out();
		for (size_t i{}; i < out.size(); ++i)
			if (out[i])
				marker.add_transition(i);
		if (marker.get_transitions().empty() && !marker.is_blocked()) {
			marker.set_blocked(true);
			f << log(time, marker.get_id(), "X");
		}
	}
}

void PetriNet::perform_tr(std::ofstream& f) {
	while (!all_blocked && !markers.empty() && !duration_pass(*this)) {
		update_tr(f);
		std::vector<std::thread> threads;
		{
			std::lock_guard lock(markers_lock);
			std::set<size_t> used;
			{
				std::lock_guard queu_lock(queue_lock);
				for (const auto& mar_ind : queue) {
					const auto& transitions = markers.at(mar_ind).get_transitions();
					std::vector<size_t> possibleTransitions;
					std::copy_if(transitions.begin(), transitions.end(), std::back_inserter(possibleTransitions),
						[&](const size_t ind) { return !used.contains(ind); });
					if (!possibleTransitions.empty()) {
						const auto it = SelectRandomly(possibleTransitions);
						threads.emplace_back([&, tr_ind = *it]() { move_mar(tr_ind, f); });
						used.insert(*it);
					}
					if (threads.size() + 1 == std::thread::hardware_concurrency())
						break;
				}
			}
			if (used.empty() && std::find_if(markers.begin(), markers.end(), [](const Marker& marker) { return !marker.is_blocked(); }) == markers.end())
				all_blocked = true;
		}
		for (auto& thread : threads)
			thread.join();
	}
}

void PetriNet::run(std::chrono::seconds dur, std::ofstream& f) {
	generate_mar_dur();
	time = time_now();
	duration = dur;
	std::thread updater([&]() { update_mar(); });
	std::thread transiter([&]() { perform_tr(f); });
	updater.join();
	transiter.join();
}
