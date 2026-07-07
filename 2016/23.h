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

//assembly simulator but with toggles

struct Command {
	int args;
	std::string name;
	std::variant<int, char> arg1, arg2;
	void Toggle() {
		if (args == 1) {
			if (name == "inc") {
				name = "dec";
			}
			else {
				name = "inc";
			}
		}
		else {
			if (name == "jnz") {
				name = "cpy";
			}
			else {
				name = "jnz";
			}
		}
	}

	void operator()(int& pos, std::unordered_map<char, int>& reg, std::vector<Command>& prog) {
		const auto isAddLoop = [](const int& pos, const std::vector<Command>& _prog) -> std::tuple<bool, char> {
			if (pos+2 >= _prog.size()) {
				return std::make_tuple(false, 0);
			}
			if (_prog[pos+1].name != "dec" || _prog[pos+2].name != "jnz") {
				return std::make_tuple(false, 0);
			}
			char addReg = std::get<char>(_prog[pos+1].arg1);
			if (std::holds_alternative<int>(_prog[pos+2].arg2) &&
				std::get<int>(_prog[pos+2].arg2) == -2 &&
				std::holds_alternative<char>(_prog[pos+2].arg1) &&
				std::get<char>(_prog[pos+2].arg1) == addReg) {
				return std::make_tuple(true, addReg);
				}
			return std::make_tuple(true, addReg);
		};
// cpy b c
// inc a
// dec c
// jnz c -2
// dec d
// jnz d -5
		const auto isMultLoop = [](const int& pos, const std::vector<Command>& _prog) -> std::tuple<bool, char, std::variant<int, char>, char> {
			if (pos+5 >= _prog.size()) {
				return std::make_tuple(false, 0, 0, 0);
			}
			if (_prog[pos+1].name != "inc" ||
				_prog[pos+2].name != "dec" ||
				_prog[pos+3].name != "jnz" ||
				_prog[pos+4].name != "dec" ||
				_prog[pos+5].name != "jnz" ||
				!std::holds_alternative<int>(_prog[pos+5].arg2) ||
				!std::holds_alternative<int>(_prog[pos+3].arg2) ||
				!std::holds_alternative<char>(_prog[pos+3].arg1) ||
				!std::holds_alternative<char>(_prog[pos+5].arg1) ||
				std::get<int>(_prog[pos+5].arg2) != -5 ||
				std::get<int>(_prog[pos+3].arg2) != -2)
				return std::make_tuple(false, 0, 0, 0);

			char tempReg = std::get<char>(_prog[pos].arg2);
			char rootReg = std::get<char>(_prog[pos+1].arg1);
			char BReg = std::get<char>(_prog[pos+4].arg1);
			std::variant<int, char> AVal = _prog[pos].arg1;

			if (
				std::get<char>(_prog[pos+2].arg1) != tempReg ||
				std::get<char>(_prog[pos+3].arg1) != tempReg ||
				std::get<char>(_prog[pos+5].arg1) != BReg) {
				return std::make_tuple(false, 0, 0, 0);
			}
			return {true, rootReg, AVal, BReg};
		};

		if (name == "cpy") {

			if (std::holds_alternative<char>(arg2)) {
				auto [success, R, A, B] = isMultLoop(pos, prog);
				if (success) {
					reg[R] += reg[B] * (std::holds_alternative<int>(A) ? std::get<int>(A) : reg[std::get<char>(A)]);
					pos+=6;
					return;
				}
				int val = std::holds_alternative<int>(arg1) ? std::get<int>(arg1) : reg[std::get<char>(arg1)];
				reg[std::get<char>(arg2)] = val;
				pos++;
			}
		}
		else if (name == "jnz") {
			int check = std::holds_alternative<int>(arg1) ? std::get<int>(arg1) : reg[std::get<char>(arg1)];
			int stride = std::holds_alternative<int>(arg2) ? std::get<int>(arg2) : reg[std::get<char>(arg2)];
			if (check != 0)
				pos+=stride;
			else
				pos++;
		}
		else if (name == "inc") {

			auto[success, address] = isAddLoop(pos, prog);
			if (success) {
				reg[std::get<char>(arg1)]+= reg[address];
				pos+=3;
				return;
			}
			if (std::holds_alternative<char>(arg1)) {
				reg[std::get<char>(arg1)]++;
			}
			pos++;
		}
		else if (name == "dec") {
			if (std::holds_alternative<char>(arg1)) {
				reg[std::get<char>(arg1)]--;
			}
			pos++;
		}
		else if (name == "tgl") {
			int offset = std::holds_alternative<int>(arg1) ? std::get<int>(arg1) : reg[std::get<char>(arg1)];
			if (pos + offset < prog.size()) {
				prog[pos + offset].Toggle();
			}
			pos++;
		}
	}
};

struct Task
{
	static inline std::vector<Command> program;
	static inline std::vector<Command> cpy;
	static inline std::unordered_map<char, int> reg;
	static void run() {

		std::ifstream inputStream("2016/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (inputStream >> line)
		{
			if (line == "cpy") {
				std::string src;
				char address;
				inputStream >> src >> address;
				program.emplace_back(2, line,
				(src[0] >= '0' && src[0] <= '9' || src[0] == '-')
					? std::variant<int, char>{std::stoi(src)}
					: std::variant<int, char>{src[0]},
					address);
			}
			if (line == "jnz") {
				std::string src;
				std::string stride;
				inputStream >> src >> stride;
				program.emplace_back(2, line,
					(src[0] >= '0' && src[0] <= '9' || src[0] == '-')
					? std::variant<int, char>{std::stoi(src)}
					: std::variant<int, char>{src[0]},
					(stride[0] >= '0' && stride[0] <= '9' || stride[0] == '-')
					? std::variant<int, char>{std::stoi(stride)}
					: std::variant<int, char>{stride[0]});
			}
			if (line == "inc" || line == "dec" || line == "tgl") {
				char address;
				inputStream >> address;
				program.emplace_back(1, line, address);
			}
		}
		inputStream.close();
		cpy = program;
		reg['a'] = 7;
		int pos = 0;
		while (pos < program.size() && pos >= 0) {
			program[pos](pos, reg, program);
		}
		std::cout << reg['a'] << std::endl;
	}
	static void runPart2() {
		program = cpy;
		reg.clear();
		reg['a'] = 12;
		int pos = 0;
		while (pos < program.size() && pos >= 0) {
			program[pos](pos, reg, program);
		}
		std::cout << reg['a'] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

cpy b c
inc a
dec c
jnz c -2
dec d
jnz d -5

this shit is equal to a += b * d

*/