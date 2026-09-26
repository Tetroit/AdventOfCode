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

//lowest risk path
struct Task
{
	static inline DynamicGrid<int> grid;
	static void run() {


		std::ifstream inputStream("2021/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		grid.fillFromStream(inputStream, [](char ch){return ch - '0';});

		int cost = grid.dijkstra(0,0, grid.getWidth()-1, grid.getHeight()-1,
			[](int val){return val;},
			[](int val){return false;});
		std::cout << cost << std::endl;
	}
	static void runPart2() {

		int initW = grid.getWidth();
		int initH = grid.getHeight();
		grid.resize(initW*5, initH*5);
		for (int x = 0; x < initW; x++) {
			for (int y = 0; y < initH; y++) {
				for (int ix=0; ix < 5; ix++) {
					for (int iy=0; iy < 5; iy++) {
						grid.set(ix * initW + x, iy * initH + y, Utils::Loop(ix + iy + grid.get(x,y), 1, 9));
					}
				}
			}
		}
		int cost = grid.aStar(0,0, grid.getWidth()-1, grid.getHeight()-1,
			[](int val){return val;},
			[](int val){return false;},
			[](int x, int y){return grid.getWidth()-1 - x + grid.getWidth()-1 - y;});
		std::cout << cost << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

Dijkstra seems pretty slow but A* is not much harder to make

*/