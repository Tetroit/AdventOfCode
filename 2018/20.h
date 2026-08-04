#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "ivec2.h"
#include "utils.h"

//walking in directions from regex

struct Task
{
	static inline std::unordered_map<ivec2, uint8_t, ivec2hash> map;
	static inline std::unordered_map<ivec2, int, ivec2hash> weights;
	static void run() {
		std::ifstream inputStream("2018/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string dirs;
		std::getline(inputStream, dirs);
		inputStream.close();

		std::vector<ivec2> branchStack{{0,0}};
		ivec2 pos = {0,0};
		bool error = false;
		for (int i=1; i<dirs.size()-1; i++) {
			switch (dirs[i]) {
				case 'N': {
					ivec2 newPos = pos+ivec2{0,1};
					map[newPos] |= 0b1000;
					map[pos] |= 0b0010;
					pos = newPos;
					break;
				}
				case 'S': {
					ivec2 newPos = pos+ivec2{0,-1};
					map[newPos] |= 0b0010;
					map[pos] |= 0b1000;
					pos = newPos;
					break;
				}
				case 'E': {
					ivec2 newPos = pos+ivec2{1,0};
					map[newPos] |= 0b0100;
					map[pos] |= 0b0001;
					pos = newPos;
					break;
				}
				case 'W': {
					ivec2 newPos = pos+ivec2{-1,0};
					map[newPos] |= 0b0001;
					map[pos] |= 0b0100;
					pos = newPos;
					break;
				}
				case '(': {
					// bool skip = false;
					// for (int j = i+1; j < dirs.size(); j++) {
					// 	if (dirs[j] == '(') break;
					// 	if (dirs[j] == ')' && dirs[j-1] == '|') {
					// 		i = j;
					// 		skip = true;
					// 		break;
					// 	}
					// }
					// if (skip) break;
					branchStack.push_back(pos);
					break;
				}
				case ')': {
					branchStack.pop_back();
					break;
				}
				case '|': {
					pos = branchStack.back();
					break;
				}
				default: {
					std::cerr << "Unresolved symbol: " << dirs[i] << std::endl;
					error = true;
					break;
				}
			}
			if (error) break;
		}

		for (auto& [key, dir] : map) {
			weights[key] = std::numeric_limits<int>::max();
		}
		weights[{0,0}] = 0;
		auto getDir = [](int dir) -> ivec2 {
			switch (dir) {
				case 0: return {1,0};
				case 1: return {0,1};
				case 2: return {-1,0};
				case 3: return {0,-1};
			};
			return {0,0};
		};
		std::deque<ivec2> toCheck{{0,0}};
		while (!toCheck.empty()) {
			ivec2 coord = toCheck.front();
			toCheck.pop_front();
			int weight = weights[coord] + 1;
			for (int i=0; i<4; i++) {
				if (! (map[coord] & (0b0001 << i))) {
					continue;
				}
				ivec2 newCoord = coord + getDir(i);
				if (weights.at(newCoord) > weight) {
					weights[newCoord] = weight;
					toCheck.push_back(newCoord);
				}
			}
		}
		int max = 0;
		for (const auto& [key, w] : weights) {
			if (w > max) max = w;
		}
		std::cout << max << std::endl;
	}
	static void runPart2() {
		int cnt = 0;
		for (const auto& [key, w] : weights) {
			if (w >= 1000) cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/