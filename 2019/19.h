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
#include "Intcode.h"
#include "utils.h"


//Tractor beam with calculation

struct Task
{
	static inline DynamicGrid<int> map;
	static inline Intcode<int> program;
	static inline std::deque<int> input;
	static inline std::deque<int> output;
	static int checkCoord(int x, int y) {
		input.push_back(x);
		input.push_back(y);
		HaltCode code;
		program.start();
		while (program.execute(input,output,code));
		program.end();
		int res = output.front();
		output.pop_front();
		return res;
	}
	static void run() {

		std::ifstream inputStream("2019/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		program = Intcode<int>(inputStream);

		inputStream.close();

		map.resize(50,50);
		map.foreachCoord([&](int x, int y) {
			map.set(x,y,checkCoord(x,y));
		});
		map.print([](int val){if (val == 1) return '#'; return '.';});
		std::cout << map.count([](int val){return val == 1;}) << std::endl;
	}
	static void runPart2() {
		const int boxSize = 100;
		int downTiltMin = 0;
		for (int x=0; x<10; x++) {
			int val = checkCoord(x, 1);
			if (val == 1) {
				downTiltMin = val;
				break;
			}
		}

		int downY = 1;
		int downX = downTiltMin+1;
		while (true) {
			downY++;
			downX+=downTiltMin;
			if (checkCoord(downX, downY) == 0) {
				downX++;
			}
			if (downY < boxSize) {
				continue;
			}
			if (checkCoord(downX+boxSize-1, downY-boxSize+1) == 1) {
				std::cout << downX * 10000 + downY - boxSize+1 << std::endl;
				break;
			}
		}
		//2650165 low
		//9880491 high
		//9760484 low
	}
};

//-------------- NOTES AREA ----------------
/*


*/