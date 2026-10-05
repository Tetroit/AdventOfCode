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
#include <array>
#include <vector>
#include "utils.h"

//moving boxes in stacks

struct Task
{
	static inline const std::regex instructionPat = std::regex(R"(move (\d+) from (\d+) to (\d+))", std::regex_constants::optimize | std::regex_constants::ECMAScript);

	static inline std::vector<std::vector<char>> initStacks;
	static inline std::vector<std::array<int, 3>> instructions;
	static void run() {

		std::ifstream inputStream("2022/05.txt");

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
				for (int i=0; i<line.size(); i+=4) {
					int stackID = i/4;
					if (std::isupper(line[i+1])) {
						if (stackID >= initStacks.size()) initStacks.resize(stackID+1);
						initStacks[stackID].insert(initStacks[stackID].begin(), line[i+1]);
					}
				}
			}
			else if (inputStage == 1) {
				std::smatch match;
				if (std::regex_match(line, match, instructionPat)) {
					instructions.push_back({std::stoi(match[1]), std::stoi(match[2])-1, std::stoi(match[3])-1});
				}
			}
		}
		inputStream.close();
		auto stacks = initStacks;
		for (auto& instruction : instructions) {
			for (int i=0; i<instruction[0]; i++) {
				char val = stacks[instruction[1]].back();
				stacks[instruction[2]].push_back(val);
				stacks[instruction[1]].pop_back();
			}
		}
		for (const auto& stack : stacks) {
			std::cout << stack.back();
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		auto stacks = initStacks;
		for (auto& instruction : instructions) {
			int insertPos = stacks[instruction[2]].size();
			for (int i=0; i<instruction[0]; i++) {
				char val = stacks[instruction[1]].back();
				stacks[instruction[2]].insert(stacks[instruction[2]].begin() + insertPos,val);
				stacks[instruction[1]].pop_back();
			}
		}
		for (const auto& stack : stacks) {
			std::cout << stack.back();
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/