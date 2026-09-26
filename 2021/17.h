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

//Physics with drag

struct Task
{
	static inline int targetXMin;
	static inline int targetXMax;
	static inline int targetYMin;
	static inline int targetYMax;
	static void run() {

		std::ifstream inputStream("2021/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		inputStream.ignore(15);
		inputStream>>targetXMin;
		inputStream.ignore(2);
		inputStream>>targetXMax;
		inputStream.ignore(4);
		inputStream>>targetYMin;
		inputStream.ignore(2);
		inputStream>>targetYMax;
		inputStream.close();

		int yStartVel = targetYMax < 0 ? - targetYMin - 1 : targetYMax;
		std::cout << yStartVel * (yStartVel + 1) / 2 << std::endl;
	}
	static bool simulate(int vx, int vy) {
		int x = 0;
		int y = 0;
		while (true) {

			x+=vx;
			y+=vy;
			vy--;
			if (vx>0) vx--;
			if (x >= targetXMin && x <= targetXMax && y >= targetYMin && y <= targetYMax) return true;
			if (y<targetYMin && vy<0) return false;
			if (x>targetXMax) return false;
			if (vx == 0 && x < targetXMin) return false;
		}
	}
	static void runPart2() {

		int maxVY = targetYMax < 0 ? - targetYMin - 1 : targetYMax;

		int cnt = 0;
		for (int vx = 0; vx <= targetXMax; vx++) {
			for (int vy = targetYMin; vy <= maxVY; vy++) {
				if (simulate(vx, vy)) cnt++;
			}
		}
		std::cout << cnt << std::endl;
		//1476
	}
};

//-------------- NOTES AREA ----------------
/*

*/