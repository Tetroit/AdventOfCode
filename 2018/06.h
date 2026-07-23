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

//voronoi noise with manhattan
struct Task
{
	static inline int maxX = 0, maxY = 0;
	static inline std::vector<std::pair<int,int>> origins;
	static void run() {

		std::ifstream inputStream("2018/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int x, y;
		char shit;
		while (inputStream >> x >> shit >> y)
		{
			origins.emplace_back(x,y);
			if (maxX < x) maxX = x;
			if (maxY < y) maxY = y;
		}
		inputStream.close();
		DynamicGrid<int> grid;
		grid.resize(maxX+1, maxY+1);
		for (int ix = 0; ix < grid.getWidth(); ix++) {
			for (int iy = 0; iy < grid.getHeight(); iy++) {
				int minDist = std::numeric_limits<int>::max();
				int minID = -1;
				bool isOnly = true;
				for (int i=0; i<origins.size(); i++) {
					int dist = abs(ix - origins[i].first) + abs(iy - origins[i].second);
					if (dist < minDist) {
						minDist = dist;
						minID = i;
						isOnly = true;
					}
					else if (dist == minDist) {
						isOnly = false;
					}
				}
				grid.set(ix, iy, isOnly ? minID : -1);
			}
		}
		std::unordered_set<int> infinite;
		std::unordered_set<int> finite;
		for (int i=0; i<grid.getWidth(); i++) {
			infinite.insert(grid.get(i,0));
			infinite.insert(grid.get(i,grid.getHeight()-1));
		}
		for (int i=0; i<grid.getHeight(); i++) {
			infinite.insert(grid.get(0,i));
			infinite.insert(grid.get(grid.getWidth()-1,i));
		}
		for (int i=0; i<origins.size(); i++) {
			if (!infinite.count(i)) {
				finite.insert(i);
			}
		}
		int maxSpread = 0;
		for (auto i : finite) {
			int cnt = grid.count([i](int el){return el == i;});
			if (cnt > maxSpread) {
				maxSpread = cnt;
			}
		}
		std::cout << maxSpread << std::endl;
		// grid.print([](int i){return i < 0 ? '.' : 'a' + i;});
	}
	static void runPart2() {
		int cnt = 0;
		const int threshold = 10000;
		for (int x = 0; x <= maxX; x++) {
			for (int y = 0; y <= maxY; y++) {
				int totalDist = 0;
				for (auto & origin : origins) {
					totalDist += abs(x - origin.first) + abs(y - origin.second);
				}
				if (totalDist < threshold)
					cnt++;
			}
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/