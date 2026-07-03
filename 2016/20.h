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
#include "utils.h"

//blacklisted ID ranges
struct Task
{
	static inline std::vector<std::pair<uint32_t, uint32_t>> ranges;
	static void merge(std::vector<std::pair<uint32_t, uint32_t>>& ranges) {
		using val_t = std::pair<uint32_t, uint32_t>;
		std::sort(ranges.begin(), ranges.end(), [](const val_t& a, const val_t& b){return a.first < b.first; });
		for (int i=0; i<ranges.size()-1; i++) {
			if (ranges[i].second >= ranges[i+1].first - 1) {
				ranges[i].second = std::max(ranges[i+1].second, ranges[i].second);
				ranges.erase(ranges.begin()+i+1);
				i--;
			}
		}
	}
	static void run() {

		std::ifstream inputStream("2016/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		uint32_t start, end;
		char shit;
		while (inputStream >> start >> shit >> end)
		{
			ranges.emplace_back(start, end);
		}
		inputStream.close();
		merge(ranges);
		std::cout << ranges[0].second + 1 << std::endl;
	}
	static void runPart2() {
		uint32_t maxID = std::numeric_limits<uint32_t>::max();
		for (auto & range : ranges) {
			maxID -= (range.second - range.first + 1);
		}
		std::cout << maxID + 1 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/