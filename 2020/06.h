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

//checking string for similarities between lines
struct Task
{
	static inline std::vector<std::vector<std::string>> answers;
	static void run() {

		std::ifstream inputStream("2020/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		{
			std::string line;
			int n = 0;
			answers.emplace_back();
			while (std::getline(inputStream, line))
			{
				if (line.empty()) {
					n++;
					answers.emplace_back();
					continue;
				}
				answers.back().emplace_back(line);
			}
			inputStream.close();
		}
		int sum = 0;
		for (auto& group : answers) {
			std::unordered_set<char> total;
			for (auto& line : group) {
				for (auto& ch : line) {
					total.insert(ch);
				}
			}
			sum += total.size();
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		int sum = 0;
		for (auto& group : answers) {
			std::unordered_map<char, int> stats;
			for (auto& line : group) {
				for (auto& ch : line) {
					stats[ch]++;
				}
			}
			for (auto& [item, cnt] : stats) {
				if (cnt == group.size()) sum++;
			}
		}
		std::cout << sum << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

*/