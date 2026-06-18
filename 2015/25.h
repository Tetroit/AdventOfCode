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

//Task about walking diagonals on a grid
struct Task
{
	//   ------> x
	//   |
	//   |
	//   v
	//   y

	static constexpr int targetX = 3083;
	static constexpr int targetY = 2978;
	static constexpr long long start = 20151125;
	static constexpr long long mult = 252533;
	static constexpr long long mod= 33554393;
	static constexpr long long next(long long i) {
		return i * mult % mod;
	};
	static void run() {
		int x = 1;
		int y = 1;
		long long num = start;
		while (x!=targetX || y!=targetY) {
			y--;
			x++;
			if (y == 0) {
				y = x;
				x = 1;
			}
			num = next(num);
		}
		std::cout << num << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/