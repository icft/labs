#include "PetriNet.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include "Random.h"

auto time_pass(const std::chrono::time_point<std::chrono::system_clock>& t) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t).count();
}

template<typename T>
concept object = requires (T x) { x.get_time(); x.get_duration(); };

template<object T>
bool duration_pass(const T& obj) {
	return time_pass(obj.get_time()) >= obj.get_duration().count();
}

std::string log(const std::chrono::time_point<std::chrono::system_clock>& timePoint, const int markerIdx, const std::string_view msg) {
	return '{' + std::to_string(time_pass(timePoint)) + "ms}<" + std::to_string(markerIdx) + ">[" + msg.data() + "]\n";
}

void PetriNet::generate_marker_duration(Marker& marker) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 1000);
	marker.set_time(std::chrono::system_clock::now());
	marker.set_duration(std::chrono::milliseconds(dist(mt)));
}

void PetriNet::generate_markers_duration() {
	std::for_each(markers.begin(), markers.end(), [&](Marker& marker) { generate_marker_duration(marker); });
}

void PetriNet::print_condition() {
	if (all_blocked)
		std::cerr << "All markers are blocked\n";
	else if (duration_pass(*this))
		std::cerr << "Time is over\n";
	else if (markers.empty())
		std::cerr << "All markers deleted\n";
}

std::vector<int> PetriNet::get_ready_input_markers(const int transitionIdx) {
	std::vector<int> inputMarkers;
	const auto& inputs = transitions.at(transitionIdx).get_inputs();
	for (int i = 0; i < inputs.size(); ++i)
		if (inputs[i]) {
			std::lock_guard<std::mutex> lock(markers_lock);
			std::lock_guard<std::mutex> queue_lock(queue_lock);
			for (const auto& marker : markers)
				if (marker.get_position() == i &&
					std::find_if(queue.begin(), queue.end(),
						[&](const int markerIdx) { return markers.at(markerIdx).get_id() == marker.get_id(); }) != queue.end())
					inputMarkers.emplace_back(marker.get_id());
		}
	return inputMarkers;
}

void PetriNet::delete_markers(std::vector<int>& m, std::ofstream& ofstr) {
	std::lock_guard<std::mutex> lock(markers_lock);
	for (const auto& markerToRemove : m) {
		auto it = std::find_if(markers.begin(), markers.end(), [&](const Marker& marker) { return marker.get_id() == markerToRemove; });
		if (it != markers.end()) {
			ofstr << log(time, markerToRemove, "-");
			positions.at(it->get_position()).remove_markers(1);
			const int pos = std::distance(markers.begin(), it);
			queue.erase(std::find(queue.begin(), queue.end(), pos));
			std::for_each(queue.begin(), queue.end(), [&](int& markerIdx) { if (markerIdx > pos) --markerIdx; });
			markers.erase(it);
		}
	}
	markers.clear();
}

void PetriNet::move_markers(const int transitionIdx, std::ofstream& ofstr) {
	auto readyInputMarkers = get_ready_input_markers(transitionIdx);
	const auto& outputs = transitions.at(transitionIdx).get_outputs();
	std::vector<int> possiblePositions;
	for (int i = 0; i < outputs.size(); ++i)
		if (outputs[i])
			possiblePositions.insert(possiblePositions.end(), outputs[i], i);
	if (possiblePositions.empty())
		delete_markers(readyInputMarkers, ofstr);
	std::lock_guard<std::mutex> lock(markers_lock);
	while (!possiblePositions.empty() && !readyInputMarkers.empty()) 	{
		const auto outputIt = SelectRandomly(possiblePositions);
		auto it = std::find_if(markers.begin(), markers.end(), [&](const Marker& marker) { return marker.get_id() == readyInputMarkers.front(); });
		if (it != markers.end()) {
			const auto position = it->get_position();
			const auto msg = std::to_string(position) + " -> " + std::to_string(*outputIt) + " | " + std::to_string(transitionIdx);
			ofstr << log(time, readyInputMarkers.front(), msg);
			positions.at(position).remove_markers(1);
			it->set_position(*outputIt);
			positions.at(*outputIt).add_markers(1);
			generate_marker_duration(*it); 
			std::lock_guard queue_lock(queue_lock);
			queue.erase(std::remove_if(queue.begin(), queue.end(),
					[&](const int markerIdx) { return markers.at(markerIdx).get_id() == readyInputMarkers.front(); }), queue.end());
			readyInputMarkers.erase(readyInputMarkers.begin());
			possiblePositions.erase(outputIt);
		}
	}
}

void PetriNet::update_markers() {
	while (!all_blocked && !markers.empty() && !duration_pass(*this)) {
		std::lock_guard lock(markers_lock);
		std::lock_guard queue_lock(queue_lock);
		for (int i = 0; i < markers.size(); ++i) {
			if (std::find(queue.begin(), queue.end(), i) != queue.end())
				continue;
			if (duration_pass(markers[i]))
				queue.push_back(i);
		}
	}
}

void PetriNet::update_transitions(std::ofstream& ofstr) {
	std::lock_guard lock(markers_lock);
	std::lock_guard queue_lock(queue_lock);
	std::for_each(markers.begin(), markers.begin(), [](Marker& marker) { marker.clear_transitions(); });
	for (const auto& markerIdx :queue) {
		auto& marker = markers.at(markerIdx);
		const auto markerPositionIdx = marker.get_position();
		const auto& position = positions.at(markerPositionIdx);
		const auto& out = position.get_out();
		for (int i = 0; i < out.size(); ++i)
			if (out[i])
				marker.add_transition(i);
		if (marker.get_transitions().empty() && !marker.is_blocked()) {
			marker.set_blocked(true);
			ofstr << log(time, marker.get_id(), "X");
		}
	} 
}

void PetriNet::perform_transitions(std::ofstream& ofstr) {
	while (!all_blocked && !markers.empty() && !duration_pass(*this)) {
		update_transitions(ofstr);
		std::vector<std::thread> threads; {
			std::lock_guard lock(markers_lock);
			std::set<int> used; {
				std::lock_guard queu_lock(queue_lock);
				for (const auto& markerIdx : queue) {
					const auto& transitions = markers.at(markerIdx).get_transitions();
					std::vector<int> possibleTransitions;
					std::copy_if(transitions.begin(), transitions.end(), std::back_inserter(possibleTransitions),
						[&](const int idx) { return !used.contains(idx); });
					if (!possibleTransitions.empty()) {
						const auto it = SelectRandomly(possibleTransitions);
						threads.emplace_back([&, transitionIdx = *it]() { move_markers(transitionIdx, ofstr); });
						used.insert(*it);
					}
					if (std::size(threads) + 1 == std::thread::hardware_concurrency())
						break;
				}
			}
			if (used.empty() && std::find_if(markers.begin(), markers.end(), [](const Marker& marker) { return !marker.is_blocked(); }) == markers.end())
				all_blocked = true;
		}
		for (auto& thread : threads)
			thread.join();
	}
	print_condition();
}


void PetriNet::run(std::chrono::seconds duration, std::ofstream& f) {
	generate_markers_duration();
	time = std::chrono::system_clock::now();
	duration = duration;
	std::thread updater([&]() { update_markers(); });
	std::thread transiter([&]() { perform_transitions(f); });
	updater.join();
	transiter.join();
}
