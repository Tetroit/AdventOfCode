#pragma once

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

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//folding paper

struct Task
{
	static inline std::unordered_set<ivec2, ivec2hash> paper;
	static inline std::vector<std::pair<char, int>> folds;
	static void run() {

		std::ifstream inputStream("2021/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int inputStage = 0;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				inputStage++;
				continue;
			}
			if (inputStage == 0) {
				auto words = Utils::split(line, ',');
				int x = std::stoi(words[0]);
				int y = std::stoi(words[1]);
				paper.emplace(ivec2{x,y});
			}
			else if (inputStage == 1) {
				char axis = line[11];
				int val = std::stoi(line.substr(13));
				folds.emplace_back(axis, val);
			}
		}
		inputStream.close();
		auto [axis, val] = folds[0];
		std::unordered_set<ivec2, ivec2hash> toRemove;
		int axisID = axis == 'x' ? 0 : 1;
		for (auto pos : paper) {
			if (pos[axisID] > val) {
				toRemove.insert(pos);
			}
		}
		for (auto pos : toRemove) {
			paper.erase(pos);
			pos[axisID] = val + val - pos[axisID];
			paper.emplace(pos);
		}
		std::cout << paper.size() << std::endl;
	}
	static void runPart2() {
		DynamicGrid<uint8_t> grid;
		for (int fold = 1; fold < folds.size(); fold++) {

			std::unordered_set<ivec2, ivec2hash> toRemove;
			auto [axis, val] = folds[fold];
			int axisID = axis == 'x' ? 0 : 1;

			for (auto pos : paper) {
				if (pos[axisID] > val) {
					toRemove.insert(pos);
				}
			}
			for (auto pos : toRemove) {
				paper.erase(pos);
				pos[axisID] = val + val - pos[axisID];
				paper.emplace(pos);
			}
		}

		ivec2 min = {INT_MAX, INT_MAX};
		ivec2 max = {INT_MIN, INT_MIN};
		for (auto pos : paper) {
			if (pos.x < min.x) {min.x = pos.x;}
			if (pos.y < min.y) {min.y = pos.y;}
			if (pos.x > max.x) {max.x = pos.x;}
			if (pos.y > max.y) {max.y = pos.y;}
		}
		grid.resize(max.x - min.x + 1, max.y - min.y + 1);
		for (auto pos : paper) {
			grid.set(pos.x, pos.y, 1);
		}
		grid.print([](uint8_t val){return val == 1 ? '#' : '.';});
	}
};

//-------------- NOTES AREA ----------------
/*

*/