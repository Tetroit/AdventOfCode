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

//Assembly simulator

enum struct Command {
	JMP,
	ACC,
	NOP,
	NAC
};

struct Task
{
	static inline std::vector<std::pair<Command, int>> commands;
	static Command fromStr(std::string str) {
		if (str == "jmp") return Command::JMP;
		if (str == "acc") return Command::ACC;
		if (str == "nop") return Command::NOP;
		return Command::NAC;
	}
	static void run() {

		std::ifstream inputStream("2020/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		{
			std::string commandStr;
			int val;
			while (inputStream >> commandStr >> val)
			{
				commands.push_back({fromStr(commandStr), val});
			}
			inputStream.close();
		}
		int ptr = 0;
		int acc = 0;
		std::vector<int> executedTimes(commands.size(), 0);
		while (ptr < commands.size() && ptr >= 0) {
			auto [command, val] = commands[ptr];
			executedTimes[ptr] += 1;
			if (executedTimes[ptr] == 2) {
				std::cout << acc << std::endl;
				break;
			}
			switch (command) {
				case Command::JMP:
					ptr+=val;
					break;
				case Command::ACC:
					acc+=val;
					ptr++;
					break;
				default:
					ptr++;
					break;
			}
		}
	}
	static void runPart2() {

		for (int i=0; i < commands.size(); i++) {

			auto& change = commands[i].first;
			if (change == Command::ACC) continue;
			if (change == Command::NOP) change = Command::JMP;
			else if (change == Command::JMP) change = Command::NOP;
			int ptr = 0;
			int acc = 0;
			std::vector<int> executedTimes(commands.size(), 0);
			bool loops = false;

			while (ptr < commands.size() && ptr >= 0) {
				auto [command, val] = commands[ptr];
				executedTimes[ptr] += 1;
				if (executedTimes[ptr] == 2) {
					loops = true;
					break;
				}
				switch (command) {
					case Command::JMP:
						ptr+=val;
						break;
					case Command::ACC:
						acc+=val;
						ptr++;
						break;
					default:
						ptr++;
						break;
				}
			}
			if (!loops && ptr == commands.size()) {
 				std::cout << acc << std::endl;
				break;
			}
			if (change == Command::NOP) change = Command::JMP;
			else if (change == Command::JMP) change = Command::NOP;

			//2015 high
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/