#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

// Numbers adjacent to symbols

struct Task
{
	static inline DynamicGrid<char> grid;
	static void run() {

		std::ifstream inputStream("2023/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		grid.fillFromStream(inputStream, DefaultConvert<char>());
		inputStream.close();

		int sum = 0;
		for (int y = 0; y < grid.getHeight(); y++) {
			for (int x = 0; x < grid.getWidth(); x++) {
				if (std::isdigit(grid.get(x,y))) {
					bool isLinked = false;
					int num = 0;
					while (x < grid.getWidth() && std::isdigit(grid.get(x,y))) {
						if (!isLinked) {
							for (auto& [nx, ny] : grid.getNeighbourCoords8(x,y)) {
								if (!std::isdigit(grid.get(nx,ny)) && grid.get(nx,ny) != '.') {
									isLinked = true;
								}
							}
						}
						num = num * 10 + grid.get(x,y) - '0';
						x++;
					}
					if (isLinked) {
						sum += num;
						std::cout << num << std::endl;
					}
				}
			}
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		std::unordered_map<ivec2, std::vector<int>, ivec2hash> gears;
		for (int y = 0; y < grid.getHeight(); y++) {
			for (int x = 0; x < grid.getWidth(); x++) {
				if (std::isdigit(grid.get(x,y))) {
					int num = 0;
					std::vector<ivec2> vecs;
					while (x < grid.getWidth() && std::isdigit(grid.get(x,y))) {
						num = num * 10 + grid.get(x,y) - '0';
						for (auto& [nx, ny] : grid.getNeighbourCoords8(x,y)) {
							if (grid.get(nx,ny) == '*') {
								if (std::find(vecs.begin(), vecs.end(), ivec2(nx,ny)) == vecs.end()) {
									vecs.emplace_back(nx,ny);
								}
							}
						}
						x++;
					}
					for (auto& vec : vecs) {
						gears[vec].push_back(num);
					}
				}
			}
		}
		int sum = 0;
		for (auto& adj : gears | std::views::values) {
			if (adj.size() != 2) continue;
			sum += adj[0] * adj[1];
		}
		std::cout << sum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/