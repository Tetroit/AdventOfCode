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

// assembly simulator with playing sounds

using numT = uint64_t;
using Reg = std::unordered_map<char, numT>;
struct ProgramContext {
	int step = 0;
	std::shared_ptr<std::deque<numT>> sent = nullptr;
	std::shared_ptr<std::deque<numT>> received = nullptr;
	int ver = 0;
	numT recover = 0;
	bool locked = false;
	int sentN = 0;
};
using Program = std::vector<std::function<void(Reg&, ProgramContext&)>>;
struct Task
{
	static inline Program program;
	static inline Reg reg;
	static std::variant<numT, char> getRef(const std::string& raw) {
		if (std::isdigit(raw[0]) || raw[0] == '-') {
			return (numT)std::stoi(raw);
		}
		return raw[0];
	}
	static void run() {
			std::ifstream inputStream("2017/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string comm;
		while (inputStream >> comm)
		{
			std::string arg;
			std::string arg2;
			char cell;
			if (comm == "snd") {
				inputStream >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([x](Reg& reg, ProgramContext& context) {
							context.sent->emplace_back(reg[x]);
							context.step++;
							context.sentN++;
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([x](Reg& reg, ProgramContext& context) {
							context.sent->emplace_back(x);
							context.step++;
							context.sentN++;
						});
					};
				}, param);
			}
			if (comm == "set") {
				inputStream >> cell >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] = reg[x];
							context.step++;
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] = x;
							context.step++;
						});
					};
				}, param);
			}
			if (comm == "add") {
				inputStream >> cell >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] += reg[x];
							context.step++;
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] += x;
							context.step++;
						});
					};
				}, param);
			}
			if (comm == "mul") {
				inputStream >> cell >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] *= reg[x];
							context.step++;
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] *= x;
							context.step++;
						});
					};
				}, param);
			}
			if (comm == "mod") {
				inputStream >> cell >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] %= reg[x];
							context.step++;
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([cell, x](Reg& reg, ProgramContext& context) {
							reg[cell] %= x;
							context.step++;
						});
					};
				}, param);
			}
			if (comm == "rcv") {
				inputStream >> arg;
				auto param = getRef(arg);
				std::visit([&](auto& x) {
					using T = std::decay_t<decltype(x)>;
					if constexpr (std::is_same_v<T, char>) {
						program.emplace_back([x](Reg& reg, ProgramContext& context) {
							if (context.ver == 0){
								if (reg[x] != 0)
									context.recover = context.received->back();
								context.step++;
							}
							else if (context.ver == 1) {
								if (context.received->empty()) {
									context.locked = true;
									return;
								}
								reg[x] = context.received->front();
								context.received->pop_front();
								context.step++;
							}
						});
					};
					if constexpr (std::is_same_v<T, numT>) {
						program.emplace_back([x](Reg& reg, ProgramContext& context) {
							if (x != 0)
								context.recover = context.received->back();
							context.step++;
						});
					};
				}, param);
			}
			if (comm == "jgz") {
				inputStream >> arg >> arg2;
				auto param = getRef(arg);
				auto param2 = getRef(arg2);
				std::function<bool(Reg&)> comp;
				if (std::holds_alternative<char>(param)) {
					cell = std::get<char>(param);
					comp = [cell](Reg& reg) {return reg[cell] > 0;};
				}
				else if (std::holds_alternative<numT>(param)) {
					numT val = std::get<numT>(param);
					comp = [val](Reg& reg) {return val > 0;};
				}
				if (std::holds_alternative<char>(param2)) {
					cell = std::get<char>(param2);
					program.emplace_back([cell, comp](Reg& reg, ProgramContext& context) {
						if (comp(reg)) context.step += (int)reg[cell];
						else context.step++;
					});
				}
				else if (std::holds_alternative<numT>(param2)) {
					numT val = std::get<numT>(param2);
					program.emplace_back([val, comp](Reg& reg, ProgramContext& context) {
						if (comp(reg)) context.step += (int)val;
						else context.step++;
					});
				}
			}
		}
		inputStream.close();
		reg.clear();
		auto stack = std::make_shared<std::deque<numT>>();
		ProgramContext ctx (0, stack, stack);
		while (ctx.step >= 0 && ctx.step < program.size() && ctx.recover == 0) {
			program[ctx.step](reg, ctx);
		}
		std::cout << ctx.recover << std::endl;
	}
	static void runPart2() {
		auto send0to1 = std::make_shared<std::deque<numT>>();
		auto send1to0 = std::make_shared<std::deque<numT>>();

		ProgramContext ctx0(0, send0to1, send1to0, 1);
		ProgramContext ctx1(0, send1to0, send0to1, 1);

		Reg reg0;
		Reg reg1;
		reg1['p'] = 1;
		while (!ctx0.locked || !ctx1.locked) {
			while (ctx0.step >= 0 && ctx0.step < program.size() && !ctx0.locked) {
				program[ctx0.step](reg0, ctx0);
			}
			if (!ctx1.received->empty())
				ctx1.locked = false;
			while (ctx1.step >= 0 && ctx1.step < program.size() && !ctx1.locked) {
				program[ctx1.step](reg1, ctx1);
			}
			if (!ctx0.received->empty())
				ctx0.locked = false;
		}
		std::cout << ctx1.sentN << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/