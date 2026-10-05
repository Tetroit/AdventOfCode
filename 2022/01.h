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
#include "utils.h"

//counting calories

struct Task
{
	static inline std::vector<int> calories;
	static void run() {

		std::ifstream inputStream("2022/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int max = 0;
		int acc = 0;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				if (acc > max) max = acc;
				calories.push_back(acc);
				acc = 0;
				continue;
			}
			acc += std::stoi(line);
		}
		calories.push_back(acc);
		inputStream.close();
		std::cout << max << std::endl;
	}
	static void runPart2() {
		int max1 = 0;
		int max2 = 0;
		int max3 = 0;
		int acc = 0;

		for (auto& cal : calories) {
			if (cal > max1) {
				max3 = max2;
				max2 = max1;
				max1 = cal;
				continue;
			}
			if (cal > max2) {
				max3 = max2;
				max2 = cal;
				continue;
			}
			if (cal > max3) {
				max3 = cal;
			}
		}
		std::cout << max1 << " + " << max2 << " + " << max3 << " = " << max1 + max2 + max3 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/