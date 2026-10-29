#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//traversing binary tree

struct Task
{
	static inline std::string instructions;
	static inline std::unordered_map<std::string, std::pair<std::string, std::string>> branches;
	static inline std::unordered_map<std::string, uint32_t> nodeIDs;
	static void run() {

		std::ifstream inputStream("2023/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}


		std::string line;
		std::getline(inputStream, instructions);
		std::getline(inputStream, line);
		while (std::getline(inputStream, line))
		{
			std::istringstream iss(line);
			std::string s, l, r;
			iss>>s;
			iss.ignore(4);
			iss>>l;
			l.pop_back();
			iss>>r;
			r.pop_back();
			branches.insert({s, {l, r}});
		}
		inputStream.close();

		std::string ptr = "AAA";
		int iPtr = 0;
		int steps = 0;
		while (true) {
			if (instructions[iPtr] == 'L') ptr = branches[ptr].first;
			else if (instructions[iPtr] == 'R') ptr = branches[ptr].second;
			iPtr = (iPtr + 1) % instructions.size();
			steps++;
			if (ptr == "ZZZ") {
				std::cout << steps << std::endl;
				break;
			}
		}
	}

	static std::pair<int, int> detectLoop (std::string ptr, std::vector<uint32_t>& iterations) {
		iterations = {nodeIDs[ptr]};
		int instr=0;
		while (true) {
			if (instructions[instr] == 'L') ptr = branches[ptr].first;
			else if (instructions[instr] == 'R') ptr = branches[ptr].second;
			instr = (instr + 1) % instructions.size();
			iterations.push_back(nodeIDs[ptr]);
			for (int i = instr % instructions.size(); i < iterations.size() - 1; i+=instructions.size()) {
				if (iterations[i] == iterations.back()) {
					return {i, iterations.size() - 1 - i};
				}
			}
		}
	}
	static void runPart2() {

		std::vector<std::string> ptrs;
		std::vector<std::vector<uint32_t>> iterations;
		std::vector<std::pair<int, int>> loops;
		std::vector<std::vector<std::pair<int, int>>> zLoops;
		std::unordered_set<int> endIDs;

		for (auto key : branches | std::views::keys) {
			if (key.back() == 'A') ptrs.push_back(key);
		}
		int id = 0;
		for (auto i = branches.begin(); i != branches.end(); i++, id++) {
			nodeIDs.emplace(i->first, id);
			if (i->first.back() == 'Z') endIDs.emplace(id);
		}

		for (auto& start : ptrs) {
			iterations.emplace_back();
			auto loop = detectLoop(start, iterations.back());
			loops.emplace_back(loop);
			zLoops.emplace_back();
			auto& history = iterations.back();
			for (int i = loop.first; i < loop.second + loop.first; i++) {
				if (endIDs.contains(history[i])) {
					zLoops.back().emplace_back(i, loop.second);
					std::cout << start << " " << i << " + n * " << loop.second << "\n";
				}
			}
		}
		std::vector<std::pair<uint64_t, uint64_t>> resLoop(loops.size());
		for (int i=0; i<zLoops.size(); i++) {
			resLoop[i] = zLoops[i][0];
		}
		auto res = Utils::chineseSolver(resLoop);
		if (res.has_value()) {
			//shit, my solver doesnt limit for only positive solutions for c = a + n * b
			//but by miracle ALL loops have A + n * A formula therefore result.first is 0
			//so we find offest result as uhh period haha
			std::cout << res.value().second << "\n";
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/