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

//following path and collecting letters

struct Task
{
	static inline DynamicGrid<int> grid;
	static void run() {

		std::ifstream inputStream("2017/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		std::string line;
		int y = 0;
		while (std::getline(inputStream, line)) {
			grid.fillFromLine(y, line, [](char x) {
				if (x == ' ') return 0;
				if (x == '|') return 1;
				if (x == '-') return 2;
				if (x == '+') return 3;
				return (int)x;
			});
			y++;
		}

		inputStream.close();

		int X = 0, Y = 0;
		int dir = 3;
		std::string res;
		for (int i=0; i<grid.getWidth(); i++) {
			if (grid.get(i, 0) == 1 || grid.get(i, 0) == 3) {
				X = i;
			}
		}

		while (true) {
			if (grid.get(X, Y) > 3) {
				res += (char)grid.get(X, Y);
			}
			if (grid.get(X, Y) != 1 && grid.get(X,Y) != 2) {
				bool wayFound = false;
				int backDir = (dir + 2)%4;
				for (int i=0; i<4; i++) {
					if (backDir == i) continue;
					auto val = grid.getInDir(X, Y, i);
					if (val && val.value() != 0) {
						dir = i;
						wayFound = true;
					}
				}
				if (!wayFound) break;
			}
			if (!grid.moveInDir(X, Y, dir)) break;
		}

		std::cout << res << std::endl;
	}
	static void runPart2() {
		int X = 0, Y = 0;
		int dir = 3;
		int steps = 0;
		std::string res;
		for (int i=0; i<grid.getWidth(); i++) {
			if (grid.get(i, 0) == 1 || grid.get(i, 0) == 3) {
				X = i;
			}
		}
		while (true) {
			if (grid.get(X, Y) != 1 && grid.get(X,Y) != 2) {
				bool wayFound = false;
				int backDir = (dir + 2)%4;
				for (int i=0; i<4; i++) {
					if (backDir == i) continue;
					auto val = grid.getInDir(X, Y, i);
					if (val && val.value() != 0) {
						dir = i;
						wayFound = true;
					}
				}
				if (!wayFound) break;
			}
			if (!grid.moveInDir(X, Y, dir)) break;
			else steps++;
		}

		std::cout << steps+1 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/