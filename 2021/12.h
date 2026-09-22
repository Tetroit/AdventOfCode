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

//Pathfinding through big and small caves

struct Task
{
	static inline std::unordered_map<std::string, std::vector<std::string>> graph;

	// static void forwardStep(const std::string& node, std::vector<std::string>& path, int& cnt) {
	// 	if (node == "end") {
	// 		cnt++;
	// 		return;
	// 	}
	// 	for (auto& vert : graph[node]) {
	// 		if (std::islower(vert[0])) {
	// 			if (std::find(path.begin(), path.end(), vert) != path.end()) {
	// 				continue;
	// 			}
	// 		}
	// 		path.push_back(vert);
	// 		forwardStep(vert, path, cnt);
	// 		path.pop_back();
	// 	}
	// }
	static void forwardStep(const std::string& node, std::unordered_map<std::string, int>& visits, int& cnt) {
		if (node == "end") {
			cnt++;
			return;
		}
		for (auto& vert : graph[node]) {
			if (std::islower(vert[0])) {
				if (visits[vert] > 0) {
					continue;
				}
			}
			visits[vert]++;
			forwardStep(vert, visits, cnt);
			visits[vert]--;
		}
	}
	static void forwardStepDouble(const std::string& node, std::unordered_map<std::string, int>& visits, int& cnt, bool doubleVisit = false) {
		if (node == "end") {
			cnt++;
			return;
		}
		for (auto& vert : graph[node]) {
			bool doubleThisTurn = false;
			if (vert == "start") continue;
			if (std::islower(vert[0])) {
				if (visits[vert] > 0) {
					if (doubleVisit) {
						continue;
					}
					doubleThisTurn = true;
					doubleVisit = true;
				}
			}
			visits[vert]++;
			forwardStepDouble(vert, visits, cnt, doubleVisit);
			visits[vert]--;
			if (doubleThisTurn) doubleVisit = false;
		}
	}
	static void run() {


		std::ifstream inputStream("2021/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			auto words = Utils::split(line, '-');
			graph[words[0]].push_back(words[1]);
			graph[words[1]].push_back(words[0]);
		}
		inputStream.close();
		std::unordered_map<std::string, int> visits {{"start", 1}};
		int cnt = 0;
		forwardStep("start", visits, cnt);
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::unordered_map<std::string, int> visits {{"start", 1}};
		int cnt = 0;
		forwardStepDouble("start", visits, cnt);
		std::cout << cnt << std::endl;
		//116447 high
	}
};

//-------------- NOTES AREA ----------------
/*

*/