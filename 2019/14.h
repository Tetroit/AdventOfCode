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

//making FUEL from reaction chain from ORE

struct Task
{
	using IngredientList = std::vector<std::pair<std::string, int>>;
	using RecipeCollection = std::unordered_map<std::string, std::pair<IngredientList, int>>;
	static inline RecipeCollection recipes;
	static inline int fuelEstimate = 0;
	static void run() {

///

		std::ifstream inputStream("2019/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			int sep = line.find("=>");
			std::string consume = line.substr(0, sep-1);
			std::string produce = line.substr(sep+3);
			std::vector<std::string> outData = Utils::split(produce, ' ');
			recipes.emplace(outData[1], std::make_pair(IngredientList(), std::stoi(outData[0])));
			auto& targetList = recipes.at(outData[1]).first;
			for (auto& str : Utils::split(consume, ", ")) {
				std::vector<std::string> data = Utils::split(str, ' ');
				targetList.emplace_back(data[1], std::stoi(data[0]));
			}
		}
		inputStream.close();

		std::unordered_map<std::string, int> requirements{{"FUEL", 1}};
		std::unordered_map<std::string, int> remainders{};
		std::deque<std::string> toSplit{"FUEL"};
		while (!toSplit.empty()) {
			auto key = toSplit.front();
			auto neededAmount = requirements.at(key) - remainders[key];
			requirements[key] = 0;
			toSplit.pop_front();
			auto& [list, producedAmount] = recipes.at(key);
			int reactionCnt = (neededAmount + producedAmount - 1) / producedAmount;
			remainders[key] = (reactionCnt * producedAmount) - neededAmount;
			for (auto& [item, cnt] : list) {
				requirements[item] += reactionCnt * cnt;
				if (item != "ORE" && std::find(toSplit.begin(), toSplit.end(), item) == toSplit.end()) toSplit.push_back(item);
			}

		}
		std::cout << requirements["ORE"] << std::endl;
		fuelEstimate = requirements["ORE"];
	}
	static void runPart2() {
		std::unordered_map<std::string, long long> requirements{{"FUEL", 1}};
		std::unordered_map<std::string, long long> totalProduced{};
		std::unordered_map<std::string, long long> remainders{};
		std::deque<std::string> toSplit{"FUEL"};
		auto produce = [&]() {
			while (!toSplit.empty()) {
				auto key = toSplit.front();
				auto neededAmount = requirements.at(key) - remainders[key];
				requirements[key] = 0;
				toSplit.pop_front();
				auto& [list, producedAmount] = recipes.at(key);
				long long reactionCnt = (neededAmount + producedAmount - 1) / producedAmount;
				remainders[key] = (reactionCnt * producedAmount) - neededAmount;
				for (auto& [item, cnt] : list) {
					requirements[item] += reactionCnt * cnt;
					totalProduced[item] += reactionCnt * cnt;
					if (item != "ORE" && std::find(toSplit.begin(), toSplit.end(), item) == toSplit.end()) toSplit.push_back(item);
				}
			}
		};
		long long totalFuel = 0;
		long long availableOre = 1'000'000'000'000LL;
		while (availableOre > fuelEstimate) {
			long long request = availableOre/fuelEstimate;
			requirements["FUEL"] = request;
			totalFuel += request;
			toSplit.clear();
			toSplit.emplace_back("FUEL");

			produce();
			availableOre -= requirements["ORE"];
			requirements["ORE"] = 0;
		}
		std::cout << totalFuel << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/