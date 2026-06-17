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

enum CommandType {
	HALF = 0,
	TRIPPLE = 1,
	INCREMENT = 2,
	JUMP = 3,
	JUMP_IF_EVEN = 4,
	JUMP_IF_1 = 5
};
struct Command {
	CommandType command;
	char reg;
	int arg;
};
using Program = std::vector<Command>;

//Registers and operations
struct Task
{
	inline static std::unordered_map<char, int> reg;
	inline static Program program;
	static bool ExecuteCommand(const Program& prog, int& it) {
		if (it >= prog.size()) {
			return false;
		}
		auto& comm = prog[it];
		switch (comm.command) {
			case CommandType::HALF: {
				reg[comm.reg]/=2;
				it++;
				break;
			}
			case CommandType::TRIPPLE: {
				reg[comm.reg]*=3;
				it++;
				break;
			}
			case CommandType::INCREMENT: {
				reg[comm.reg]++;
				it++;
				break;
			}
			case CommandType::JUMP: {
				it += comm.arg;
				break;
			}
			case CommandType::JUMP_IF_EVEN: {
				if (reg[comm.reg] % 2 == 0) {
					it += comm.arg;
				}
				else it++;
				break;
			}
			case CommandType::JUMP_IF_1: {
				if (reg[comm.reg] == 1) {
					it += comm.arg;
				}
				else it++;
				break;
			}
		}
		return true;
	}
	static void run() {

		std::ifstream inputStream("2015/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			int arg = 0;
			char reg;
			std::string pattern = line.substr(0,3);
			if (pattern == "jmp") {
				arg = std::stoi(line.substr(4));
				program.push_back({JUMP, 0, arg});
			}
			else {
				reg = line[4];
				if (pattern == "hlf") {
					program.push_back({HALF, reg});
				}
				else if (pattern == "tpl") {
					program.push_back({TRIPPLE, reg});
				}
				else if (pattern == "inc") {
					program.push_back({INCREMENT, reg});
				}
				else
				{
					arg = std::stoi(line.substr(7));
					if (pattern == "jie") {
						program.push_back({JUMP_IF_EVEN, reg, arg});
					}
					else if (pattern == "jio") {
						program.push_back({JUMP_IF_1, reg, arg});
					}
					else {
						std::cerr << "Unrecognized command type\n";
					}
				}
			}
		}
		inputStream.close();

		int ptr = 0;

		while (ExecuteCommand(program, ptr));

		for (auto& [cell, val] : reg) {
			std::cout << cell << " : " << val << std::endl;
		}
	}
	static void runPart2() {
		int ptr = 0;
		reg.clear();

		reg['a'] = 1;
		while (ExecuteCommand(program, ptr));

		for (auto& [cell, val] : reg) {
			std::cout << cell << " : " << val << std::endl;
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/