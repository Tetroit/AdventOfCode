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
#include <array>
#include "utils.h"

//analyzing ranges
struct Task
{
	static inline std::vector<std::array<int,4>> ranges;
	static void run() {

		std::ifstream inputStream("2022/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::istringstream iss(line);
			int a1, a2, b1, b2;
			char shit;
			iss >> a1 >> shit >> a2 >> shit >> b1 >> shit >> b2;
			ranges.push_back(std::array<int,4>{a1, a2, b1, b2});
		}
		inputStream.close();

		int cnt = 0;
		for (auto& range : ranges) {
			if (range[0] <= range[2] && range[1] >= range[3] ||
				range[0] >= range[2] && range[1] <= range[3]) cnt++;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		int cnt = 0;
		for (auto& range : ranges) {
			if (!(range[1] < range[2] || range[3] < range[0]))
				cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/