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

//calculating orbits
struct Task
{
	using Graph = std::unordered_map<std::string, std::vector<std::string>>;
	static inline Graph orbits;
	static inline std::unordered_map<std::string, std::string> parents;
	static int countOrbits(Graph& graph, std::string obj, int depth) {
		int cnt = 0;
		for (auto& child : graph[obj]) {
			cnt += countOrbits(graph, child, depth+1) + depth + 1;
		}
		return cnt;
	}
	static void run() {

		std::ifstream inputStream("2019/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			auto split = line.find(')');
			auto parent = line.substr(0, split);
			auto child = line.substr(split+1);
			orbits[parent].emplace_back(child);
			parents[child] = parent;
		}
		parents["COM"] = ".";
		inputStream.close();
		std::cout << countOrbits(orbits, "COM", 0) << std::endl;
	}
	static int depthOf (std::string node) {
		int i = 0;
		std::string& current = node;
		while (parents[current] != ".") {
			current = parents[current];
			i++;
		}
		return i;
	}
	static void runPart2() {
		int youDep = depthOf("YOU");
		int sanDep = depthOf("SAN");
		int youPath = 0;
		int sanPath = 0;
		std::string startYou = parents["YOU"], startSan = parents["SAN"];
		if (youDep > sanDep) {
			while (youDep != sanDep) {
				youPath++;
				startYou = parents[startYou];
				youDep--;
			}
		}
		else if (sanDep > youDep) {
			while (sanDep != youDep) {
				sanPath++;
				startSan = parents[startSan];
				sanDep--;
			}
		}
		while (startYou != startSan) {
			startYou = parents[startYou];
			startSan = parents[startSan];
			youPath++;
			sanPath++;
		}
		std::cout << youPath + sanPath << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/