#pragma once

#include <algorithm>
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

//hex grid

struct Task
{
	static void run() {

		std::ifstream inputStream("2017/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		std::vector<std::string> dirs = Utils::split(line, ',');
		inputStream.close();
		int x = 0;
		int y = 0;
		for (auto& dir : dirs) {
			if (dir == "n") {
				y++;
			}
			if (dir == "s") {
				y--;
			}
			if (dir == "nw") {
				x--;
			}
			if (dir == "se") {
				x++;
			}
			if (dir == "sw") {
				x--;
				y--;
			}
			if (dir == "ne") {
				x++;
				y++;
			}
		}
		int res = x + y;
		if ((x < 0) == (y < 0)) {
			res -= std::min(x,y);
		}
		std::cout << res << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		std::vector<std::string> dirs = Utils::split(line, ',');
		inputStream.close();
		int x = 0;
		int y = 0;
		int max = 0;
		for (auto& dir : dirs) {
			if (dir == "n") {
				y++;
			}
			if (dir == "s") {
				y--;
			}
			if (dir == "nw") {
				x--;
			}
			if (dir == "se") {
				x++;
			}
			if (dir == "sw") {
				x--;
				y--;
			}
			if (dir == "ne") {
				x++;
				y++;
			}
			int dist = x + y;
			if ((x < 0) == (y < 0)) {
				dist -= std::min(x,y);
			}
			if (max < dist) {
				max = dist;
			}
		}
		std::cout << max << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
hex grid is also quad grid!

	x       x
x       x       x
	x       x
x       x       x
	x       x

|  `x-_ |  `x-_
x-_ |  `x-_ |  `x
|  `x-_ |  `x-_
x-_ |  `x-_ |  `x
|  `x-_ |  `x

*/