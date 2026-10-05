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

//counting x and cycles and drawing texture

struct Task
{
	enum struct Command {
		NOOP,
		ADDX
	};
	static inline std::vector<std::pair<Command, int>> commands;
	static void run() {

		std::ifstream inputStream("2022/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string command;
		while (inputStream >> command)
		{
			if (command == "addx") {
				int arg;
				inputStream >> arg;
				commands.emplace_back(Command::ADDX, arg);
			}
			if (command == "noop") {
				commands.emplace_back(Command::NOOP, 0);
			}
		}
		inputStream.close();
		int next = 20;
		int score = 0;
		int cycle = 0;
		int x = 1;
		for (auto& [comm, arg] : commands) {
			if (comm == Command::NOOP) {
				cycle++;
				continue;
			}
			if (comm == Command::ADDX) {
				cycle+=2;
				if (cycle >= next) {
					score += next * x;
					next += 40;
				}
				x+=arg;
			}
		}
		std::cout << score << std::endl;
	}
	static void runPart2() {
		int sx = 1;
		int opId = 0;
		int nextOpCycle = commands[0].first == Command::ADDX ? 2 : 1;
		Grid<40,6,bool> screen;
		screen.clear(false);
		for (int y=0; y<6; y++) {
			for (int x=0; x<40; x++) {
				int cycle = y * 40 + x;
				if (nextOpCycle == cycle && opId < commands.size()) {
					if (commands[opId].first == Command::ADDX) {
						sx += commands[opId].second;
						nextOpCycle+=2;
					}
					else
						nextOpCycle++;
					opId++;
				}
				if ( abs(sx - x) <= 1)
					screen.set(x,y,true);
			}
		}
		screen.print([](bool val){return val ? '#' : '.';});
	}
};

//-------------- NOTES AREA ----------------
/*

*/