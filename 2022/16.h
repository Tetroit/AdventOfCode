#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//connecting tunnels and valves teaching elephant to open them
struct Task
{
	static inline std::unordered_map<std::string, std::vector<std::string>> map;
	static inline std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> optimized;
	static inline std::unordered_map<std::string, int> pressures;
	static inline const std::regex pattern = std::regex(R"(Valve (\w+) has flow rate=(\d+); (?:tunnels lead to valves|tunnel leads to valve) (.+))");

	static int openMax(
		std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>& graph,
		std::unordered_set<std::string>& opened,
		const std::string& curr, int step, const int& cap) {

		int max = 0;
		std::unordered_set<std::string> combo;
		for (const auto& [cost, next] : graph[curr]) {
			if (cost + step >= cap) continue;
			int score = 0;
			if (!opened.contains(next)) {
				opened.insert(next);
				score = pressures[next] * (cap - step - cost) + openMax(graph, opened, next, step+cost+1, cap);
				opened.erase(next);
			}
			if (score > max) {
				max = score;
			}
		}
		return max;
	}
	static int openMax2(
		std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>& graph,
		std::unordered_set<std::string>& opened,
		const std::string& curr, int step, const int& cap) {

		if (step >= cap) {
			return openMax(graph, opened, "AA", 1, cap);
		};

		int max = 0;
		std::unordered_set<std::string> combo;
		for (const auto& [cost, next] : graph[curr]) {
			int score = 0;
			if (!opened.contains(next)) {
				opened.insert(next);
				score = pressures[next] * (cap - step - cost) + openMax2(graph, opened, next, step+cost+1, cap);
				opened.erase(next);
			}
			if (score > max) {
				max = score;
			}
		}
		int stopNow = openMax(graph, opened, "AA", 1, cap);
		if (stopNow > max)
			max = stopNow;
		return max;
	}

	static std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> optimizeGraph() {
		std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> graph;
		for (const auto &src: map | std::views::keys) {
			if (pressures[src] == 0 && src != "AA") continue;
			std::unordered_map<std::string, int> steps;
			for (const auto &node: map | std::views::keys) {
				steps[node] = INT_MAX;
			}
			steps[src] = 0;
			std::deque<std::string> queue{src};
			while (!queue.empty()) {
				std::string curr = queue.front();
				queue.pop_front();
				int w = steps[curr] + 1;
				for (auto& next : map[curr]) {
					if (steps[next] > w) {
						steps[next] = w;
						queue.push_back(next);
					}
				}
			}
			for (auto& [node, cnt] : steps) {
				if (node == src) continue;
				if (pressures[node] == 0) continue;
				graph[src].emplace_back(cnt, node);
			}
		}
		return graph;
	}

	static void run() {

		std::ifstream inputStream("2022/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_match(line, match, pattern);
			std::vector<std::string> dsts = Utils::split(match[3], ", ");
			map[match[1]] = {dsts};
			pressures[match[1]] = std::stoi(match[2]);
		}
		inputStream.close();

		std::unordered_set<std::string> opened;
		std::unordered_set<std::string> resOpened;
		optimized = optimizeGraph();
		std::cout << openMax(optimized, opened, "AA", 1, 30) << std::endl;
	}
	static void runPart2() {
		std::unordered_set<std::string> opened;
		std::unordered_set<std::string> resOpened;
		std::unordered_set<std::string> resOpened2;
		int score = openMax2(optimized, opened, "AA", 1, 26);
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/