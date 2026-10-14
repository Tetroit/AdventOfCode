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

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//climbing in mountains

struct Task
{
	static inline DynamicGrid<int> grid;
	static inline ivec2 start;
	static inline ivec2 end;
	static void run() {

		std::ifstream inputStream("2022/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		grid.fillFromStream(inputStream, [](char ch) {
			if (ch == 'S') return -1;
			if (ch == 'E') return 26;
			return ch - 'a';
		});
		inputStream.close();
		grid.foreachCoord([&](int x, int y) {
			if (grid.get(x,y) == -1) {
				start = {x, y};
				grid.set(x,y, 0);
			}
			if (grid.get(x,y) == 26) {
				end = {x, y};
				grid.set(x,y, 25);
			}
		});
		DynamicGrid<int> weights;
		weights.resize(grid.getWidth(), grid.getHeight());
		weights.clear(INT_MAX);
		weights.set(start.x, start.y, 0);
		std::deque<ivec2> queue{start};

		int dist = INT_MAX;
		while (!queue.empty()) {
			auto pos = queue.front();
			queue.pop_front();
			int height = grid.get(pos.x, pos.y);
			if (height == 25) {
				dist = weights.get(pos.x, pos.y);
				break;
			}
			for (auto& [x,y] : grid.getNeighbourCoords4(pos.x, pos.y)) {
				int newHeight = grid.get(x, y);
				int newWeight = weights.get(pos.x, pos.y) + 1;
				if (newHeight - height > 1) continue;
				if (weights.get(x,y) > newWeight) {

					bool inserted = false;
					for (int i = queue.size()-1; i>=0; i--) {
						const auto& coord = queue[i];
						int w = weights.get(coord.x, coord.y);

						if (coord.x == x && coord.y == y && w > newWeight) {
							queue.erase(queue.begin() + i);
							continue;
						}

						if (w <= newWeight) {
							inserted = true;
							queue.insert(queue.begin() + i + 1, {x,y});
							break;
						}
					}
					if (!inserted) {
						queue.emplace_back(x,y);
					}

					weights.set(x, y, newWeight);
				}
			}
		}
		std::cout << dist << std::endl;
		for (int y = 0; y < grid.getHeight(); y++) {
			for (int x = 0; x < grid.getWidth(); x++) {
				if (weights.get(x,y) == INT_MAX)
					std::cout << "-" << '\t';
				else
					std::cout << weights.get(x,y) << '\t';
			}
			std::cout << '\n';
		}
		std::cout << std::endl;
	}
	static void runPart2() {

		int minDist = INT_MAX;
		for (int sy = 0; sy < grid.getHeight(); sy++) {
			for (int sx = 0; sx < grid.getWidth(); sx++) {
				if (grid.get(sx, sy) == 0) {

					DynamicGrid<int> weights;
					weights.resize(grid.getWidth(), grid.getHeight());
					weights.clear(INT_MAX);
					weights.set(start.x, start.y, 0);
					std::deque<ivec2> queue{start};
					start = {sx, sy};
					while (!queue.empty()) {
						auto pos = queue.front();
						queue.pop_front();
						int height = grid.get(pos.x, pos.y);
						if (height == 25) {
							int dist = weights.get(pos.x, pos.y);
							if (dist < minDist) {minDist = dist;}
							break;
						}
						for (auto& [x,y] : grid.getNeighbourCoords4(pos.x, pos.y)) {
							int newHeight = grid.get(x, y);
							int newWeight = weights.get(pos.x, pos.y) + 1;
							if (newHeight - height > 1) continue;
							if (weights.get(x,y) > newWeight) {

								bool inserted = false;
								for (int i = queue.size()-1; i>=0; i--) {
									const auto& coord = queue[i];
									int w = weights.get(coord.x, coord.y);

									if (coord.x == x && coord.y == y && w > newWeight) {
										queue.erase(queue.begin() + i);
										continue;
									}

									if (w <= newWeight) {
										inserted = true;
										queue.insert(queue.begin() + i + 1, {x,y});
										break;
									}
								}
								if (!inserted) {
									queue.emplace_back(x,y);
								}

								weights.set(x, y, newWeight);
							}
						}
					}
				}
			}
		}

		std::cout << minDist << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/