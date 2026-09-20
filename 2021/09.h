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

//scanning landscape on a grid with lowest positions

struct Task
{
	static inline DynamicGrid<int> caves;
	static void run() {

		std::ifstream inputStream("2021/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		caves.fillFromStream(inputStream, [](char ch){return ch - '0';});
		int cnt = 0;
		caves.foreachCoord([&](int x, int y) {
			int cell = caves.get(x,y);
			bool localMin = true;
			for (auto& n : caves.getNeighbourCoords4(x,y)) {
				if (cell >= caves.get(n.first, n.second)) {
					localMin = false;
					break;
				}
			}
			if (localMin) cnt += 1 + cell;
		});
		std::cout << cnt << std::endl;
	}

	static void runPart2() {
		DynamicGrid<int> basinId;
		basinId.resize(caves.getWidth(), caves.getHeight());
		basinId.clear(-1);
		int newId = 0;
		std::vector<int> sizes;

		auto fillBasin = [&](int sx, int sy, int id) -> int {
			std::deque<std::pair<int, int>> q;
			q.emplace_back(sx, sy);
			int size = 0;
			while (!q.empty()) {
				auto [x, y] = q.front();
				q.pop_front();
				if (basinId.get(x,y) != -1) continue;
				basinId.set(x,y, newId);
				size++;
				for (auto [nx,ny] : caves.getNeighbourCoords4(x,y)) {
					if (caves.get(nx, ny) == 9) continue;
					q.emplace_back(nx, ny);
				}
			}
			return size;
		};

		for (int x=0; x<caves.getWidth(); x++) {
			for (int y=0; y<caves.getHeight(); y++) {
				if (caves.get(x,y) == 9) continue;
				if (basinId.get(x,y) != -1) continue;

				sizes.push_back(fillBasin(x,y,newId++));
			}
		}
		std::sort(sizes.begin(), sizes.end(), std::greater());
		std::cout << sizes[0] * sizes[1] * sizes[2] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/