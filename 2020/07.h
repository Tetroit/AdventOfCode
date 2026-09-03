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

//bags containing bags

struct Task
{
	static inline std::unordered_map<std::string, std::unordered_map<std::string, int>> map;
	static inline std::unordered_map<std::string, std::unordered_map<std::string, int>> reverse;
	static void run() {

		std::regex pattern(R"((.+) bags contain (.+).)", std::regex_constants::optimize | std::regex_constants::ECMAScript);
		std::ifstream inputStream("2020/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_match(line, match, pattern);
			std::string key = match[1].str();
			std::string content = match[2].str();
			map[key];
			if (content == "no other bags") continue;
			std::vector<std::string> entries = Utils::split(content, ", ");
			for (auto& entry : entries) {
				auto sep = entry.find_first_of(' ');
				int val = std::stoi(entry.substr(0, sep));
				std::string bag = entry.substr(sep + 1, entry.find_last_of(' ') - sep - 1);
				map[key][bag] = val;
				reverse[bag][key] = val;
			}
		}
		inputStream.close();
		std::unordered_set<std::string> visited{"shiny gold"};
		std::deque<std::string> possibleBags{"shiny gold"};
		int cnt = 0;
		while (!possibleBags.empty()) {
			std::string bag = possibleBags.front();
			possibleBags.pop_front();
			for (auto& [entry, n] : reverse[bag]) {
				if (!visited.contains(entry)) {
					cnt++;
					possibleBags.push_back(entry);
					visited.insert(entry);
				}
			}
		}
		std::cout << cnt << std::endl;
	}
	static int countBags(const std::string& key) {
		int cnt = 0;
		for (auto& [bag, n] : map[key]) {
			cnt += (countBags(bag) + 1) * n;
		}
		return cnt;
	}
	static void runPart2() {
		std::cout << countBags("shiny gold") << std::endl;
		//14112 low
	}
};

//-------------- NOTES AREA ----------------
/*

*/