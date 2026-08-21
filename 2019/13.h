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
#include "vec.h"


//brick breaker game with drawing the level

struct Task
{
	static inline Intcode<int> program;
	static inline std::unordered_map<ivec2, int, ivec2hash> map;
	static inline DynamicGrid<int> display;
	static inline int nBricks = 0;
	static void run() {

		std::ifstream inputStream("2019/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		program = Intcode<int>(inputStream);
		std::deque<int> input;
		std::deque<int> output;
		HaltCode hcode;
		program.start();
		while (program.execute(input, output, hcode));
		program.end();

		int cnt = 0;
		for (int i=0; i<output.size(); i+=3) {
			int x = output[i];
			int y = output[i+1];
			int type = output[i+2];
			map[{x,y}]=type;
			if (type == 2) cnt++;
		}
		std::cout << cnt << std::endl;
		int minX = INT_MAX;
		int minY = INT_MAX;
		int maxX = INT_MIN;
		int maxY = INT_MIN;
		for (const auto& [pos, val] : map) {
			if (minX > pos.x) minX = pos.x;
			if (maxX < pos.x) maxX = pos.x;
			if (minY > pos.y) minY = pos.y;
			if (maxY < pos.y) maxY = pos.y;
		}
		display.resize(maxX - minX + 1, maxY - minY + 1);
		for (const auto& [pos, val] : map) {
			display.set(pos.x - minX, pos.y - minX, val);
		}
		display.print([](int val) {
			switch (val) {
				case 0: return '.';
				case 1: return 'N';
				case 2: return '#';
				case 3: return '_';
				case 4: return 'o';
			}
		});

		std::cout << cnt << std::endl;
		nBricks = cnt;
	}
	static int padAutopilot(int padX, int ballX) {
		if (ballX < padX) return -1;
		if (ballX > padX) return 1;
		return 0;
	}
	static void runPart2() {
		static constexpr bool MANUAL_MODE = false;
		static constexpr bool ENABLE_DISPLAY = false;


		std::deque<int> input{2};
		std::deque<int> output;
		HaltCode hcode;
		program.program[0] = 2;
		program.start();

		while (program.executeUntilInput(input, output, hcode));
		int bricks = 0;
		while (true) {

			while (program.executeUntilInput(input, output, hcode));
			if (hcode == HaltCode::ERROR) std::cerr << "HaltCode error!\n";

			int score = 0;
			ivec2 ballPos = {0,0};
			ivec2 padPos = ivec2(0, 0);
			for (int i=0; i<output.size(); i+=3) {
				int x = output[i];
				int y = output[i+1];
				int type = output[i+2];
				if (x == -1 && y == 0) {
					score = type;
					continue;
				}
				if (type == 3) padPos = ivec2(x, y);
				else if (type == 4) ballPos = ivec2(x, y);
				display.set(x,y,type);
			}
			if constexpr (ENABLE_DISPLAY) {
				display.print([](int val) {
					switch (val) {
						case 0: return '.';
						case 1: return 'N';
						case 2: return '#';
						case 3: return '_';
						case 4: return 'o';
					}
				});
				std::cout << "Score: " << score << std::endl;
				std::cout << "Bricks left: " << bricks << std::endl;
			}
			if constexpr (MANUAL_MODE) {
				std::cout << score << std::endl;
				int consoleInput = 0;
				std::cin >> consoleInput;
				if (consoleInput == -2) break;
				input.push_back(consoleInput);
			}
			else {
				bricks = display.count([](int val){return val == 2;});
				if (bricks == 0) {
					std::cout << score << std::endl;
					break;
				}
				input.push_back(padAutopilot(padPos.x, ballPos.x));
			}
		}
		program.end();
	}
};

//-------------- NOTES AREA ----------------
/*

*/