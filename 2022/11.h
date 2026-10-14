#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//juggling monkis


struct Task
{

	struct Monkey {

		static inline const std::regex monkeyPattern = std::regex(R"(Monkey (\d+):
  Starting items: (.+)
  Operation: new = (.+)
  Test: divisible by (\d+)
    If true: throw to monkey (\d+)
    If false: throw to monkey (\d+))");

		std::function<long long(long long)> operation;
		int id;
		int condition;
		int dstIfTrue;
		int dstIfFalse;
		int inspections = 0;
		std::deque<long long> items;

		explicit Monkey(const std::string& block) {
			std::smatch match;
			std::regex_match(block, match, monkeyPattern);
			id = std::stoi(match[1]);
			for (auto& item : Utils::split(match[2], ", ")) {
				items.push_back(std::stoi(item));
			}
			{
				const auto& opStr = match[3].str();
				auto words = Utils::split(opStr, ' ');
				long long operand1 = words[0] == "old" ? -1 : std::stoll(words[0]);
				char op = words[1][0];
				long long operand2 = words[2] == "old" ? -1 : std::stoll(words[2]);
				if (op == '+') {
					operation = [operand1, operand2, op](long long x) {
						return (operand1 == -1 ? x : operand1) + (operand2 == -1 ? x : operand2);
					};
				}
				else if (op == '*') {
					operation = [operand1, operand2, op](long long x) {
						return (operand1 == -1 ? x : operand1) * (operand2 == -1 ? x : operand2);
					};
				}
			}
			condition = std::stoi(match[4].str());
			dstIfTrue = std::stoi(match[5].str());
			dstIfFalse = std::stoi(match[6].str());
		}

		bool inspectNext(long long& outVal, int& dst) {
			if (items.empty()) return false;
			inspections++;
			outVal = items.front();
			items.pop_front();
			outVal = operation(outVal) / 3;
			dst = outVal % condition == 0 ? dstIfTrue : dstIfFalse;
			return true;
		}
		bool inspectNext2(long long& outVal, int& dst, int maxMult) {
			if (items.empty()) return false;
			inspections++;
			outVal = items.front();
			items.pop_front();
			outVal = operation(outVal) % maxMult;
			dst = outVal % condition == 0 ? dstIfTrue : dstIfFalse;
			outVal = outVal == 0 ? condition : outVal;
			return true;
		}
	};

	static inline std::vector<Monkey> monkeys;
	static void run() {

		std::ifstream inputStream("2022/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string block = "";
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				block.pop_back();
				monkeys.emplace_back(block);
				block = "";
				continue;
			}
			block.append(line + '\n');
		}
		block.pop_back();
		monkeys.emplace_back(block);
		inputStream.close();

		for (int i=0; i<20; i++) {
			for (auto& monkey : monkeys) {
				long long val = 0;
				int dst = 0;
				while (monkey.inspectNext(val, dst)) {
					monkeys[dst].items.push_back(val);
				}
			}
		}
		int max1 = 0;
		int max2 = 0;
		for (auto& monkey : monkeys) {
			if (monkey.inspections > max1) {
				max2 = max1;
				max1 = monkey.inspections;
				continue;
			}
			if (monkey.inspections > max2) {
				max2 = monkey.inspections;
				continue;
			}
		}
		std::cout << max1 * max2 << std::endl;
	}
	static void runPart2() {
		monkeys.clear();
		std::ifstream inputStream("2022/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string block = "";
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				block.pop_back();
				monkeys.emplace_back(block);
				block = "";
				continue;
			}
			block.append(line + '\n');
		}
		block.pop_back();
		monkeys.emplace_back(block);
		inputStream.close();

		int maxMult = 1;
		for (auto& monkey : monkeys) {
			maxMult *= monkey.condition;
		}
		for (int i=0; i<10000; i++) {
			for (auto& monkey : monkeys) {
				long long val = 0;
				int dst = 0;
				while (monkey.inspectNext2(val, dst, maxMult)) {
					monkeys[dst].items.push_back(val);
				}
			}
		}
		int max1 = 0;
		int max2 = 0;
		for (auto& monkey : monkeys) {
			std::cout << monkey.inspections << std::endl;
			if (monkey.inspections > max1) {
				max2 = max1;
				max1 = monkey.inspections;
				continue;
			}
			if (monkey.inspections > max2) {
				max2 = monkey.inspections;
				continue;
			}
		}
		std::cout << (long long)max1 * max2 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/