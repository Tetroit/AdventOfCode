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

//counting fuel for each mass
struct Task
{
	static void run() {

		std::ifstream inputStream("2019/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int mass;
		int cnt =0;
		while (inputStream >> mass)
		{
			cnt+=mass/3-2;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2019/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int mass;
		int total = 0;
		while (inputStream >> mass)
		{
			int iteration = mass;
			while (true) {
				iteration = iteration/3-2;
				if (iteration <= 0) break;
				total += iteration;
			}
		}
		inputStream.close();
		std::cout << total << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/