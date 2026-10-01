#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Sorting amphipods

struct Task {
	struct Config {
		const std::vector<int>* x;
		const std::vector<int>* y;
		const std::vector<char>* zone;
		const std::vector<std::vector<int>>* graph;
		int roomDepth = 2;
		int hallwayLength = 7;

		int roomBegin(int id) const {
			return hallwayLength + roomDepth * id;
		}
		int roomEnd(int id) const {
			return hallwayLength + roomDepth * (id + 1) - 1;
		}
		bool isHallway(int id) const {
			return id < hallwayLength;
		}
		int roomID(int id) const {
			return (id - hallwayLength) / roomDepth;
		}
	};

	static inline const std::vector<int> posToHallway	{0,1,3,5,7,9,10,	2,2,4,4,6,6,8,8};
	static inline const std::vector<int> exitSteps		{0,0,0,0,0,0,0,		1,2,1,2,1,2,1,2};
	static inline const std::vector<char> dstType		{0,0,0,0,0,0,0,		'A','A','B','B','C','C','D','D'};
	static inline const std::vector<std::vector<int>> posGraph {
		{1},
		{0,2,7},
		{1,3,7,9},
		{2,4,9,11},
		{3,5,11,13},
		{4,6,13},
		{5},
		{1,2,8},
		{7},
		{2,3,10},
		{9},
		{3,4,12},
		{11},
		{4,5,14},
		{13}
	};
	static constexpr std::array<int, 4> costs			{1,10,100,1000};

	static inline const std::vector<int> posToHallway2	{0,1,3,5,7,9,10,	2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8};
	static inline const std::vector<int> exitSteps2		{0,0,0,0,0,0,0,		1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
	static inline const std::vector<char> dstType2		{0,0,0,0,0,0,0,		'A','A','A','A','B','B','B','B','C','C','C','C','D','D','D','D'};

	static inline const std::vector<std::vector<int>> posGraph2 {
		{1},
		{0,2,7},
		{1,3,7,11},
		{2,4,11,15},
		{3,5,15,19},
		{4,6,19},
		{5},

		{1,2,8},
		{7,9},
		{8,10},
		{9},

		{2,3,12},
		{11,13},
		{12,14},
		{13},

		{3,4,16},
		{15,17},
		{16,18},
		{17},

		{4,5,20},
		{19,21},
		{20,22},
		{21}
	};


	static inline Config config{&posToHallway, &exitSteps, &dstType, &posGraph, 2, 7};

	static int moveCost(int pos1, int pos2, char type) {
		int hallway1 = config.x->at(pos1);
		int hallway2 = config.x->at(pos2);
		int steps = config.y->at(pos1) + config.y->at(pos2) + abs(hallway1 - hallway2);
		int cost = costs[type - 'A'];
		return cost * steps;
	}
	

	static int nextRoomSlot(const std::vector<char>& state, int room) {
		for (int i = config.roomEnd(room); i >= config.roomBegin(room); i--) {
			char type = state[i];
			if (type == 0) return i;
			if (type != room + 'A') return -1;
		}
	}
	static bool shouldBeMoved(const std::vector<char>& state, int pos) {
		if (state[pos] == 0) return false;

		if (config.isHallway(pos)) return true;

		int type = state[pos] - 'A';
		int room = config.roomID(pos);
		if (room != type) return true;
		if (nextRoomSlot(state, room) == -1) return true;
		return false;
	}

	//given the path is clear, is this change allowed by rules
	static bool canTravel(int from, int to, const std::vector<char>& state) {
		char type = state[from];
		if (type == 0) return false;
		if (config.isHallway(from)) {
			if (config.isHallway(to)) return false;
			if (config.roomID(to) != (type - 'A')) return false;
			return nextRoomSlot(state, type - 'A') == to;
		}
		return config.isHallway(to);
	}

	//mainly graph traversal
	static std::vector<int> getDestinations(const std::vector<char>& state, int pos) {
		std::vector<int> queue;
		std::vector<bool> visited;
		visited.resize(config.graph->size(), false);
		std::vector<int> destinations;
		queue.push_back(pos);

		while (!queue.empty()) {
			int node = queue.back();
			queue.pop_back();
			visited[node] = true;
			if (canTravel(pos, node, state)) destinations.push_back(node);
			for (auto& n : config.graph->at(node)) {
				if (visited[n]) continue;
				if (state[n] != 0) continue;
				queue.push_back(n);
			}
		}
		return destinations;
	}
	static bool check(const std::vector<char>& state) {
		for (int id = 0; id < 4; id++) {
			for (int i=config.roomBegin(id); i<=config.roomEnd(id); i++) {
				if (state.at(i) == 0 || state.at(i) - 'A' != id) return false;
			}
		}
		return true;
	}
	static void print(const std::vector<char>& state) {
		auto c = [](char type) {
			if (type == 0) return '.';
			return type;
		};
		std::cout <<
			"#############\n"
			"#"<<c(state[0])<<c(state[1])<<"."<<c(state[2])<<"."<<c(state[3])<<"."<<c(state[4])<<"."<<c(state[5])<<c(state[6])<<"#\n"
			"###"<<c(state[7])<<"#"<<c(state[9])<<"#"<<c(state[11])<<"#"<<c(state[13])<<"###\n"
			"  #"<<c(state[8])<<"#"<<c(state[10])<<"#"<<c(state[12])<<"#"<<c(state[14])<<"#  \n"
			"  #########  \n" << std::endl;
	}
	static void print2(const std::vector<char>& state) {
		auto c = [](char type) {
			if (type == 0) return '.';
			return type;
		};
		std::cout <<
			"#############\n"
			"#"<<c(state[0])<<c(state[1])<<"."<<c(state[2])<<"."<<c(state[3])<<"."<<c(state[4])<<"."<<c(state[5])<<c(state[6])<<"#\n"
			"###"<<c(state[7])<<"#"<<c(state[11])<<"#"<<c(state[15])<<"#"<<c(state[19])<<"###\n"
			"  #"<<c(state[8])<<"#"<<c(state[12])<<"#"<<c(state[16])<<"#"<<c(state[20])<<"#  \n"
			"  #"<<c(state[9])<<"#"<<c(state[13])<<"#"<<c(state[17])<<"#"<<c(state[21])<<"#  \n"
			"  #"<<c(state[10])<<"#"<<c(state[14])<<"#"<<c(state[18])<<"#"<<c(state[22])<<"#  \n"
			"  #########  \n" << std::endl;
	}
	static uint64_t encode(const std::vector<char>& state) {
		uint64_t result = 0;
		std::array<int, 4> types{};
		for (int i = 0; i < 15; i++) {
			char type = state[i];
			if (type == 0) continue;
			type-='A';
			result |= (i << (type * 2 + types[type]) * 4);
			types[type]++;
		}
		return result;
	}
	static int arrange(const std::vector<char> &state, std::unordered_map<uint64_t, int>& cache) {
		int best = INT_MAX;
		for (int i=0; i<15; i++) {
			if (state[i] == 0) continue;
			if (!shouldBeMoved(state,i)) continue;
			for (auto& dst : getDestinations(state, i)) {
				std::vector<char> newState = state;
				newState[i] = 0;
				newState[dst] = state[i];
				// print(newState);
				int cost = moveCost(i, dst, state[i]);
				if (check(newState))
					return cost;
				int next = 0;
				uint64_t newHash = encode(newState);
				if (cache.contains(newHash)) {
					next = cache[newHash];
				}
				else {
					next = arrange(newState, cache);
					cache[newHash] = next;
				}
				if (next == INT_MAX) continue;
				int score = cost + next;
				if (score < best) {
					best = score;
				}
			}
		}
		return best;
	}
	static void run() {

		std::ifstream inputStream("2021/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<char> cells{};
		cells.resize(15);
		int row = 0;
		while (std::getline(inputStream, line))
		{
			int cell = 0;
			for (char i : line) {
				if (std::isupper(i)) {
					cells[cell * 2 + row + 7] = i;
					cell++;
				}
			}
			if (cell != 0) row++;
		}
		inputStream.close();
		std::unordered_map<uint64_t, int> cache;
		std::cout << arrange(cells, cache) << std::endl;

	}

	struct LargeMap {
		uint64_t val1;
		uint64_t val2;
		explicit LargeMap(const std::vector<char>& state) {
			val1 = 0;
			val2 = 0;
			std::array<int, 4> types{};
			for (int i = 0; i < 23; i++) {
				char type = state[i];
				if (type == 0) continue;
				type-='A';
				if (type < 2) {
					val1 |= (uint64_t(i) << (type * config.roomDepth + types[type]) * 5);
				}
				else {
					val2 |= (uint64_t(i) << ((type - 2) * config.roomDepth + types[type]) * 5);
				}
				types[type]++;
			}
		}

		bool operator==(const LargeMap & other) const {
			return val1 == other.val1 && val2 == other.val2;
		}
	};
	struct LargeMapHash {
		size_t operator () (const LargeMap& map) const {
			std::hash<uint64_t> hasher;
			return hasher(map.val1) << 1 | hasher(map.val2);
		}
	};

	static int arrange2(const std::vector<char> &state, std::unordered_map<LargeMap, int, LargeMapHash>& cache) {
		int best = INT_MAX;
		for (int i=0; i<23; i++) {
			if (state[i] == 0) continue;
			if (!shouldBeMoved(state,i)) continue;
			for (auto& dst : getDestinations(state, i)) {
				std::vector<char> newState = state;
				newState[i] = 0;
				newState[dst] = state[i];
				// print2(newState);
				int cost = moveCost(i, dst, state[i]);
				if (check(newState))
					return cost;
				int next = 0;
				auto newHash = LargeMap(newState);
				if (cache.contains(newHash)) {
					next = cache[newHash];
				}
				else {
					next = arrange2(newState, cache);
					cache[newHash] = next;
				}
				if (next == INT_MAX) continue;
				int score = cost + next;
				if (score < best) {
					best = score;
				}
			}
		}
		return best;
	}
	static void runPart2() {

		config.hallwayLength = 7;
		config.roomDepth = 4;
		config.x = &posToHallway2;
		config.y = &exitSteps2;
		config.zone = &dstType2;
		config.graph = &posGraph2;
		std::ifstream inputStream("2021/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<char> cells{};
		cells.resize(config.roomDepth * 4 + config.hallwayLength);
		int row = 0;
		while (std::getline(inputStream, line))
		{
			int cell = 0;
			for (char i : line) {
				if (std::isupper(i)) {
					cells[cell * config.roomDepth + row + 7] = i;
					cell++;
				}
			}
			if (cell != 0) row = 3;
		}

		inputStream.close();

		cells[8] = 'D'; cells[12] = 'C'; cells[16] = 'B'; cells[20] = 'A';
		cells[9] = 'D'; cells[13] = 'B'; cells[17] = 'A'; cells[21] = 'C';

		std::unordered_map<LargeMap, int, LargeMapHash> cache;
		std::cout << arrange2(cells, cache) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

#############
#01.2.3.4.56#
###7#9#B#D###
  #8#A#C#E#
  #########

o kurwa mac... part 2

#############
#01.2.3.4.56#
###7#B#F#J###
  #8#C#G#K#
  #9#D#H#L#
  #A#E#I#M#
  #########

there can be a funny upgrade to actually do A* with state graph instead of dijkstra
using energy needed to put each amphipod into its spot ignoring borders and rules as heuristic function
but thats extra calculations so nahhh

*/