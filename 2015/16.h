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

//Detect which of 500 aunts sent the present

struct Task
{
	static constexpr int N = 500;
	inline static std::vector<std::unordered_map<std::string, int>> items;
	inline static std::unordered_map<std::string, int> target{
		{"children", 3},
		{"cats", 7},
		{"samoyeds", 2},
		{"pomeranians", 3},
		{"akitas", 0},
		{"vizslas", 0},
		{"goldfish", 5},
		{"trees", 3},
		{"cars", 2},
		{"perfumes", 1}
	};
	static void run() {

		std::ifstream inputStream("2015/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		std::regex listPattern(R"(Sue (\d+): (.*))");
		std::regex itemPattern(R"((\w+): (\d+))");
		std::string line;
		items.resize(N);
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_search(line, match, listPattern);
			int id = std::stoi(match[1].str());
			std::string listRaw = match[2].str();
			auto list = Utils::split(listRaw, ", ");
			for (auto & entry: list) {
				std::vector<std::string> itemRaw = Utils::split(entry, ": ");
				std::string name = itemRaw[0];
				int amount = std::stoi(itemRaw[1]);
				items[id-1].emplace(name, amount);
			}
		}
		inputStream.close();
		for (int i = 0; i<items.size(); i++) {
			auto& dict = items.at(i);
			bool match = true;
			for (auto& [item, amount]: dict) {
				auto it = target.find(item);
				if (it == target.end()) {
					continue;
				}
				if (amount != it->second) {
					match = false;
					break;
				}
			}
			if (match) {
				std::cout << i+1 << std::endl;
			}
		}
	}
	static void runPart2() {
		for (int i = 0; i<items.size(); i++) {
			auto& dict = items.at(i);
			bool match = true;
			for (auto& [item, amount]: dict) {
				auto it = target.find(item);
				if (it == target.end()) {
					continue;
				}
				if (item == "trees" || item == "cats") {
					if (amount <= it->second) {
						match = false;
						break;
					}
				}
				else if (item == "pomeranians" || item == "goldfish") {
					if (amount >= it->second) {
						match = false;
						break;
					}
				}
				else if (amount != it->second) {
					match = false;
					break;
				}
			}
			if (match) {
				std::cout << i+1 << std::endl;
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/