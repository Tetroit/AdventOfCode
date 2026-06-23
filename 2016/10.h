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

//bots giving min and max to other bots or outputs

struct BotTask;

struct Pointer {
	enum Mode {
		NUL,
		VAL,
		MIN,
		MAX
	};
	int val = 0;
	Mode mode = NUL;
	explicit operator bool() const {
		return mode != NUL;
	}
};

struct BotTask {
private:
	Pointer& next() {
		return !a ? a : b;
	}
	void yield(std::unordered_map<int, BotTask>& map) {
		int valA = GetFrom(a, map);
		int valB = GetFrom(b, map);
		cacheMin = std::min(valA, valB);
		cacheMax = std::max(valA, valB);
	}
public:
	Pointer a;
	Pointer b;
	std::optional<int> cacheMin;
	std::optional<int> cacheMax;

	void SetInstruction(Pointer::Mode mode, int val) {
		Pointer& p = next();
		p.mode = mode;
		p.val = val;
	}
	static int GetFrom(const Pointer& ptr, std::unordered_map<int, BotTask>& map) {
		if (ptr.mode == Pointer::VAL) return ptr.val;
		if (ptr.mode == Pointer::MIN) return map[ptr.val].GetMin(map);
		if (ptr.mode == Pointer::MAX) return map[ptr.val].GetMax(map);
		return 0;
	}

	int GetMin(std::unordered_map<int, BotTask>& map) {
		if (!cacheMin) yield(map);
		return cacheMin.value();
	}
	int GetMax(std::unordered_map<int, BotTask>& map) {
		if (!cacheMax) yield(map);
		return cacheMax.value();
	}
};
struct Task
{
	static inline std::unordered_map<int, BotTask> botLogistics;
	static inline std::unordered_map<int, Pointer> output;
	static int findResponsible (int a, int b) {
		if (a > b) std::swap(a, b);
		for (auto& [id, task] : botLogistics) {
			int botA = task.GetMin(botLogistics);
			int botB = task.GetMax(botLogistics);
			if (botA == a && botB == b) {
				return id;
			}
		}
		return -1;
	}
	static void run() {

		std::ifstream inputStream("2016/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::regex pattern (R"(value (\d+) goes to bot (\d+)|bot (\d+) gives low to (output|bot) (\d+) and high to (output|bot) (\d+))");
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_match(line, match, pattern);
			if (match[1].matched) {
				auto& task = botLogistics[std::stoi(match[2])];
				task.SetInstruction(Pointer::VAL, std::stoi(match[1]));
			}
			if (match[3].matched) {
				int bot = std::stoi(match[3]);
				std::string type1 = match[4];
				int id1 = std::stoi(match[5]);
				std::string type2 = match[6];
				int id2 = std::stoi(match[7]);
				auto deduce = [](const std::string& type, Pointer::Mode mode, int dst, int src) {
					if (type == "output") {
						Pointer& ptr = output[dst];
						ptr.mode = mode;
						ptr.val = src;
					}
					else {
						botLogistics[dst].SetInstruction(mode, src);
					}
				};
				deduce(type1, Pointer::MIN, id1, bot);
				deduce(type2, Pointer::MAX, id2, bot);
			}
		}
		inputStream.close();
		std::cout << findResponsible(17,61) << "\n";
	}
	static void runPart2() {
		auto getOutput = [](int cell) {
			Pointer& ptr = output[cell];
			if (ptr.mode == Pointer::MIN) {
				return botLogistics[ptr.val].GetMin(botLogistics);
			}
			if (ptr.mode == Pointer::MAX) {
				return botLogistics[ptr.val].GetMax(botLogistics);
			}
			return 0;
		};
		int res = getOutput(0) * getOutput(1) * getOutput(2);
		std::cout << res << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*

*/