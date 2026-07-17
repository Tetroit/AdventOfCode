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

// Assembly simulator


using numT = long long;
using Reg = std::unordered_map<char, numT>;

struct ProgramContext {
	int step = 0;
	int mulCnt = 0;
};

struct Task
{
	static inline std::vector<std::function<void(Reg&, ProgramContext&)>> program;
	static inline Reg reg;

	static std::variant<numT, char> getRef(const std::string& raw) {
		if (std::isdigit(raw[0]) || raw[0] == '-') {
			return (numT)std::stoi(raw);
		}
		return raw[0];
	}
	static void run() {

		std::ifstream inputStream("2017/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string comm;
		std::string arg, arg2;
		while (inputStream >> comm) {
			if (comm == "set") {
				char cell;
				inputStream >> cell >> arg2;
				auto param2 = getRef(arg2);
				if (std::holds_alternative<char>(param2)) {
					char cell2 = std::get<char>(param2);
					program.emplace_back([cell, cell2](Reg& reg, ProgramContext& ctx) {
						reg[cell] = reg[cell2];
						ctx.step++;

					});
				}
				else if (std::holds_alternative<numT>(param2)) {
					numT val = std::get<numT>(param2);
					program.emplace_back([cell, val](Reg& reg, ProgramContext& ctx) {
						reg[cell] = val;
						ctx.step++;
					});
				}
			}
			if (comm == "sub") {
				char cell;
				inputStream >> cell >> arg2;
				auto param2 = getRef(arg2);
				if (std::holds_alternative<char>(param2)) {
					char cell2 = std::get<char>(param2);
					program.emplace_back([cell, cell2](Reg& reg, ProgramContext& ctx) {
						reg[cell] -= reg[cell2];
						ctx.step++;
					});
				}
				else if (std::holds_alternative<numT>(param2)) {
					numT val = std::get<numT>(param2);
					program.emplace_back([cell, val](Reg& reg, ProgramContext& ctx) {
						reg[cell] -= val;
						ctx.step++;
					});
				}
			}
			if (comm == "mul") {
				char cell;
				inputStream >> cell >> arg2;
				auto param2 = getRef(arg2);
				if (std::holds_alternative<char>(param2)) {
					char cell2 = std::get<char>(param2);
					program.emplace_back([cell, cell2](Reg& reg, ProgramContext& ctx) {
						reg[cell] *= reg[cell2];
						ctx.mulCnt++;
						ctx.step++;
					});
				}
				else if (std::holds_alternative<numT>(param2)) {
					numT val = std::get<numT>(param2);
					program.emplace_back([cell, val](Reg& reg, ProgramContext& ctx) {
						reg[cell] *= val;
						ctx.mulCnt++;
						ctx.step++;
					});
				}
			}
			if (comm == "jnz") {
				inputStream >> arg >> arg2;
				auto param = getRef(arg);
				auto param2 = getRef(arg2);
				std::function<bool(Reg&)> comp;
				if (std::holds_alternative<char>(param)) {
					auto cell = std::get<char>(param);
					comp = [cell](Reg& reg) {return reg[cell] != 0;};
				}
				else if (std::holds_alternative<numT>(param)) {
					numT val = std::get<numT>(param);
					comp = [val](Reg& reg) {return val != 0;};
				}
				if (std::holds_alternative<char>(param2)) {
					auto cell = std::get<char>(param2);
					program.emplace_back([cell, comp](Reg& reg, ProgramContext& ctx) {
						if (comp(reg))
							ctx.step += (int)reg[cell];
						else ctx.step++;
					});
				}
				else if (std::holds_alternative<numT>(param2)) {
					auto val = std::get<numT>(param2);
					program.emplace_back([val, comp](Reg& reg, ProgramContext& ctx) {
						if (comp(reg))
							ctx.step += (int)val;
						else ctx.step++;
					});
				}
			}
		}
		inputStream.close();
		ProgramContext context;
		while (context.step < program.size() && context.step >= 0) {
			program[context.step](reg, context);
		}
		std::cout << context.mulCnt << std::endl;
		std::cout << reg['h'] << std::endl;
		reg.clear();
	}
	static void runPart2() {
		int h = 0;
		int b = 108100;
		int c = b + 17000;
		for (; b <= c; b+=17) {
			for (int e = 2; e < b; e++) {
				if (b % e == 0) {
					h++;
					break;
				}
			}
		}
		std::cout << h << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

--3--
set f 1
set d 2
--2--
set e 2
--1--
set g d
mul g e
sub g b
jnz g 2
set f 0
sub e -1
set g e
sub g b
jnz g -8
--1--
sub d -1
set g d
sub g b
jnz g -13
--2--
jnz f 2
sub h -1
set g b
sub g c
jnz g 2
jnz 1 3
sub b -17
jnz 1 -23
--3--

while (true) {
	bool f = false;
	for (int d = 2; d < b; d++) {
		for (int e = 2; e < b; e++) {
			if (d * e == b)
				f = true;
		}
	}
	if (f) h++;
	if (b == c) break;
	b += 17;
}

CASE 1:
	b = 81
	c = 81
CASE 2:
	b = 108100
	c = 108100 + 17000

basically just an algo that checks if the number is NOT prime with gaps of 17
for the particular input 100
*/