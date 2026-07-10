#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//connecting programs

struct Task
{
	static inline std::unordered_map<int, std::vector<int>> connections;
	static inline std::unordered_map<int, int> groups;

	static void run() {

		std::ifstream inputStream("2017/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			int id;
			std::istringstream ss(line);
			ss >> id;
			ss.ignore(4);
			int link;
			while (ss >> link) {
				connections[id].push_back(link);
				ss.ignore(2);
			}
		}
		inputStream.close();
		groups[0] = 0;
		int groupID = 0;
		std::deque<int> spread {0};
		int cnt0 = 1;
		while (!spread.empty()) {
			for (auto& connection : connections[spread.front()]) {
				if (groups.contains(connection)) continue;
				spread.push_back(connection);
				groups[connection] = groups[spread.front()];
				cnt0++;
			}
			spread.pop_front();
		}
		std::cout << cnt0 << std::endl;
	}
	static void runPart2() {
		groups.clear();
		int groupID = 0;
		int cntGr = 0;
		for (const auto &id: connections | std::views::keys) {
			if (groups.contains(id)) continue;
			cntGr++;
			std::deque<int> spread {id};
			while (!spread.empty()) {
				for (auto& connection : connections[spread.front()]) {
					if (groups.contains(connection)) continue;
					spread.push_back(connection);
					groups[connection] = groups[spread.front()];
				}
				spread.pop_front();
			}
		}
		std::cout << cntGr << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/