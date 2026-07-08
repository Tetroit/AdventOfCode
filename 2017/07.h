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

struct Tower {
	std::string name;
	int weight;
	int weightRec;
	std::vector <std::string> children;
	std::string parent = "";
};

struct Task
{
	static inline const std::regex parentPat = std::regex(R"((\w+) \((\d+)\) -> (.+))");
	static inline const std::regex leafPat = std::regex(R"((\w+) \((\d+)\))");
	static inline std::unordered_map <std::string, Tower> towers;
	static inline std::string root;

	static bool isBalanced(std::string node) {

		Tower& tower = towers[node];

		if (towers[tower.children[0]].weightRec * tower.children.size() + tower.weight != tower.weightRec) {
			return false;
		}
		return true;
	}

	static int searchImbalance(std::string node, int deviation = 0) {
		std::unordered_map<std::string, int> deviations;
		std::deque<std::string> toCheck;
		toCheck.push_back(node);
		while (!toCheck.empty()) {
			Tower& tower = towers[toCheck.front()];
			toCheck.pop_front();

			if (!isBalanced(tower.name)) {
				std::vector<std::string> culprits;
				int truth = -1;

				//identify issues
				for (int i=0; i < tower.children.size() - 1; i++) {
					bool match = false;
					const std::string& child = tower.children[i];
					for (int j = 0; j < tower.children.size(); j++) {
						if (i == j) continue;
						if (towers[child].weightRec == towers[tower.children[j]].weightRec) {
							match = true;
							truth = towers[child].weightRec;
						}
					}
					if (!match) {
						culprits.push_back(child);
					}
				}

				// if only 1 wrong and it is balanced, we found it, post forward otherwise
				if (culprits.size() == 1) {
					const auto& child = culprits[0];
					deviation = towers[child].weightRec - truth;
					if (!isBalanced(child)) {
						toCheck.push_back(child);
					}
					else {
						return towers[child].weight - deviation;
					}
				}
				//if 2 are wrong it can be any of them and depends on deviation from earlier results
				else {
					const auto& child1 = towers[culprits[0]];
					const auto& child2 = towers[culprits[1]];
					if (isBalanced(child1.name) && isBalanced(child2.name)) {
						if (child1.weightRec - child2.weightRec == deviation) return child2.weight;
						return child1.weight;
					}
					toCheck.push_back(child1.name);
					toCheck.push_back(child2.name);
				}
			}
		}
	}
	static int scanWeights(std::string node) {
		int accum = towers[node].weight;
		for (const auto& child : towers[node].children) {
			accum += scanWeights(child);
		}
		towers[node].weightRec = accum;
		return accum;
	}
	static void run() {

		std::ifstream inputStream("2017/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::smatch match;
		while (std::getline(inputStream, line))
		{
			Tower tower;
			if (std::regex_match(line, match, parentPat)) {
				tower.name = match[1];
				tower.weight = std::stoi(match[2]);
				tower.children = Utils::split(match[3], ", ");
			}
			else if (std::regex_match(line, match, leafPat)) {
				tower.name = match[1];
				tower.weight = std::stoi(match[2]);
			}
			towers.emplace(tower.name, tower);
		}
		inputStream.close();
		for (auto& [name, tower] : towers) {
			for (auto& child : tower.children) {
				towers[child].parent = name;
			}
		}
		for (auto& [name, tower] : towers) {
			if (tower.parent.empty()) {
				std::cout << name << std::endl;
				root = name;
			}
		}
	}
	static void runPart2() {
		scanWeights(root);
		int res = searchImbalance(root);
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/