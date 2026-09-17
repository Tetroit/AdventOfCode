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

//cheapest alignment of crabs

struct Task
{
	static inline std::vector<int> crabs;
	static void run() {

		std::ifstream inputStream("2021/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
		{
			crabs.push_back(num);
			inputStream.ignore(1);
		}
		inputStream.close();

		int min = INT_MAX, max = INT_MIN;
		for (int i = 0; i<crabs.size(); i++) {
			if (crabs[i] < min) min = crabs[i];
			if (crabs[i] > max) max = crabs[i];
		}
		int minFuel = INT_MAX;
		for (int i = min; i <= max; i++ ) {

			int fuel = 0;
			for (auto num : crabs) {
				fuel += abs(num - i);
			}
			if (fuel < minFuel) minFuel = fuel;
		}
		std::cout << minFuel << std::endl;
	}
	static void runPart2() {
		int min = INT_MAX, max = INT_MIN;
		for (int i = 0; i<crabs.size(); i++) {
			if (crabs[i] < min) min = crabs[i];
			if (crabs[i] > max) max = crabs[i];
		}
		int minFuel = INT_MAX;
		for (int i = min; i <= max; i++ ) {

			int fuel = 0;
			for (auto num : crabs) {
				int dist = abs(num - i);
				fuel += dist * (dist + 1) / 2;
			}
			if (fuel < minFuel) minFuel = fuel;
		}
		std::cout << minFuel << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/