#pragma once

#include <array>
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

//making robots to dig geodes

struct Task
{
	struct Blueprint {
		int n;
		std::array<std::array<int,4>,4> req;
		std::array<int,4> robotCaps{0,0,0,0};
		Blueprint (int n, int oreOre, int oreClay, int oreObs, int clayObs, int oreGeo, int obsGeo) : n(n), req() {
			req[0][0] = oreOre;
			req[1][0] = oreClay;
			req[2][0] = oreObs;
			req[3][0] = oreGeo;
			req[2][1] = clayObs;
			req[3][2] = obsGeo;
			robotCaps[0] = std::max(oreOre, std::max(oreClay, std::max(oreObs, oreGeo)));
			robotCaps[1] = clayObs;
			robotCaps[2] = obsGeo;
			robotCaps[3] = INT_MAX;
		}
	};

	static inline const std::regex pattern = std::regex(R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)", std::regex_constants::ECMAScript | std::regex_constants::optimize);
	static inline std::vector<Blueprint> blueprints;
	static void advance(std::vector<int>& robots, std::vector<int>& resources, int n = 1) {
		for (int i=0; i<robots.size(); i++) {
			resources[i] += robots[i] * n;
		}
	}
	static int timeToBuild(const std::array<int,4>& req, const std::vector<int>& robots, const std::vector<int>& resources) {
		int time = 0;
		for (int i=0; i<req.size(); i++) {
			if (req[i] <= resources[i]) continue;
			if (robots[i] == 0) return -1;
			int timeReq = (req[i] - resources[i] + robots[i] - 1)/robots[i];
			if (timeReq > time) time = timeReq;
		}
		return time;
	}
	static int maxGeodes(const Blueprint& bp, const std::vector<int>& robots, const std::vector<int>& resources, int step, int cap) {
		int max = robots[3] * (cap - step) + resources[3];
		for (int i=0; i<4; i++) {
			if (robots[i] >= bp.robotCaps[i]) continue;
			int time = timeToBuild(bp.req[i], robots, resources);
			if (time == -1) continue;
			if (step + time + 1 >= cap) continue;
			auto nres = resources;
			auto nrob = robots;
			advance(nrob, nres, 1 + time);
			nrob[i] ++;
			for (int j=0; j<4; j++) {
				nres[j] -= bp.req[i][j];
			}
			int score = maxGeodes(bp, nrob, nres, step + time + 1, cap);
			if (score > max) {
				max = score;
			}
		}

		return max;
	}
	static void run() {

		std::ifstream inputStream("2022/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, pattern)) {
				blueprints.emplace_back(
				std::stoi(match[1]),
				std::stoi(match[2]),
				std::stoi(match[3]),
				std::stoi(match[4]),
				std::stoi(match[5]),
				std::stoi(match[6]),
				std::stoi(match[7]));
			}
		}
		inputStream.close();
		int sum = 0;
		for (const auto& bp : blueprints) {
			std::vector<int> robots{1,0,0,0};
			std::vector<int> resources{0,0,0,0};
			int max = maxGeodes(bp,robots,resources,0,24);
			sum += max * bp.n;
			std::cout << max << std::endl;
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		int mult = 1;
		for (int i=0; i<3; i++) {
			Blueprint& bp = blueprints[i];
			std::vector<int> robots{1,0,0,0};
			std::vector<int> resources{0,0,0,0};

			mult *= maxGeodes(bp,robots,resources, 0,32);
		}
		std::cout << mult << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/