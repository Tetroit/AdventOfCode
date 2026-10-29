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

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//shortest path between galaxies
struct Task
{
	static inline std::vector<ivec2> galaxies;
	static inline DynamicGrid<char> map;
	static inline std::vector<std::vector<int>> graph;
	static inline std::unordered_set<int> emptyX;
	static inline std::unordered_set<int> emptyY;
	static void run() {

		std::ifstream inputStream("2023/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		map.fillFromStream(inputStream, DefaultConvert<char>());
		map.foreachCoord([](int x, int y) {
			if (map.get(x,y) == '#') {
				galaxies.emplace_back(x,y);
			}
		});
		inputStream.close();
		for (int i=0; i<map.getWidth();i++) {
			emptyY.insert(i);
		}
		for (int i=0; i<map.getHeight();i++) {
			emptyX.insert(i);
		}
		for (auto& galaxy : galaxies) {
			emptyX.erase(galaxy.x);
			emptyY.erase(galaxy.y);
		}
		int total = 0;
		for (int i=0; i<galaxies.size()-1; i++) {
			auto& galaxy = galaxies[i];
			for (int j = i+1; j < galaxies.size(); j++) {
				auto& other = galaxies[j];
				int minX = std::min(other.x,galaxy.x);
				int minY = std::min(other.y,galaxy.y);
				int maxX = std::max(other.x,galaxy.x);
				int maxY = std::max(other.y,galaxy.y);
				int dist = maxX - minX + maxY - minY;
				for (auto& gap : emptyX) {
					if (gap < maxX && gap > minX) dist++;
				}
				for (auto& gap : emptyY) {
					if (gap < maxY && gap > minY) dist++;
				}
				total += dist;
			}
		}
		std::cout << total << std::endl;
	}
	static void runPart2() {
		long long total = 0;
		for (int i=0; i<galaxies.size()-1; i++) {
			auto& galaxy = galaxies[i];
			for (int j = i+1; j < galaxies.size(); j++) {
				auto& other = galaxies[j];
				int minX = std::min(other.x,galaxy.x);
				int minY = std::min(other.y,galaxy.y);
				int maxX = std::max(other.x,galaxy.x);
				int maxY = std::max(other.y,galaxy.y);
				long long dist = maxX - minX + maxY - minY;
				for (auto& gap : emptyX) {
					if (gap < maxX && gap > minX) dist+=999999;
				}
				for (auto& gap : emptyY) {
					if (gap < maxY && gap > minY) dist+=999999;
				}
				total += dist;
			}
		}
		std::cout << total << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/