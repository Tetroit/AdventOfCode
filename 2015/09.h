#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

//The task about Santa traveling around the cities and
//finding the shortest distance to travel and visit every city
struct Task
{
	typedef std::unordered_map<std::string,std::vector<std::pair<std::string,int>>>  worldMap;
	inline static worldMap map;

	// static int weightedDijkstra(const worldMap& pMap, const std::string& start) {
	// 	std::unordered_map<std::string, int> distances;
	// 	std::vector<std::string> queue;
	// 	distances[start] = 0;
	// 	queue.push_back(start);
	// 	while (!queue.empty()) {
	// 		const auto pointA = queue.begin();
	// 		const auto& currentWeight = distances.at(*pointA);
	//
	// 		for (auto& destination : pMap.at(*pointA)) {
	// 			auto loc = distances.find(destination.first);
	// 			if (loc == distances.end() || destination.second + currentWeight < loc->second) {
	// 				distances.emplace(destination.first, destination.second + currentWeight);
	// 				int insertIndex = 0;
	// 				for (int i=0; i<queue.size(); i++) {
	// 					if (distances[queue[i]] <= destination.second + currentWeight) {
	// 						insertIndex = i;
	// 					}
	// 				}
	// 				queue.insert(queue.begin() + insertIndex, destination.first);
	// 			}
	// 		}
	// 		queue.erase(queue.begin());
	// 	}
	// 	int max = 0;
	// 	for (auto& [place, dist] : distances) {
	// 		if (max < dist) {
	// 			max = dist;
	// 		}
	// 	}
	// 	return max;
	// }
	static int findBestRoute(const worldMap& pMap, const std::string& start, std::vector<std::string>* stack = nullptr) {
		if (stack == nullptr) {
			stack = new std::vector<std::string>;
		}
		if (stack->size() >= pMap.size() - 1) {
			return 0;
		}
		stack->push_back(start);
		int min = std::numeric_limits<int>::max();
		for (auto& [place, dist] : pMap.at(start)) {
			if (std::find(stack->begin(), stack->end(), place) != stack->end()) {
				continue;
			}
			int yielded = findBestRoute(pMap, place, stack);
			if (yielded + dist < min) {
				min = yielded + dist;
			}
		}
		stack->pop_back();
		if (stack->empty()) {
			delete stack;
		}
		return min;
	}
	static int findWorstRoute(const worldMap& pMap, const std::string& start, std::vector<std::string>* stack = nullptr) {
		if (stack == nullptr) {
			stack = new std::vector<std::string>;
		}
		if (stack->size() >= pMap.size() - 1) {
			return 0;
		}
		stack->push_back(start);
		int max = 0;
		for (auto& [place, dist] : pMap.at(start)) {
			if (std::find(stack->begin(), stack->end(), place) != stack->end()) {
				continue;
			}
			int yielded = findWorstRoute(pMap, place, stack);
			if (yielded + dist > max) {
				max = yielded + dist;
			}
		}
		stack->pop_back();
		if (stack->empty()) {
			delete stack;
		}
		return max;
	}
	static void run() {

		std::ifstream inputStream("09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::regex distancePattern(R"((\w+) to (\w+) = (\d+))");
		std::smatch distanceMatch;
		while (std::getline(inputStream, line))
		{
			if (std::regex_match(line, distanceMatch, distancePattern)) {
				map[distanceMatch[1].str()].emplace_back(distanceMatch[2].str(), std::stoi(distanceMatch[3].str()));
				map[distanceMatch[2].str()].emplace_back(distanceMatch[1].str(), std::stoi(distanceMatch[3].str()));
			}
		}
		inputStream.close();

		int min = std::numeric_limits<int>::max();
		for (auto& [key,val]: map) {
			int dist = findBestRoute(map, key);
			if (dist < min) {
				min = dist;
			}
		}
		std::cout << min << "\n";
	}
	static void runPart2() {

		int max = 0;
		for (auto& [key,val]: map) {
			int dist = findWorstRoute(map, key);
			if (dist > max) {
				max = dist;
			}
		}
		std::cout << max << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*
wait graph pathfinding doesnt work here ._.
screw it, bruteforce it is
*/