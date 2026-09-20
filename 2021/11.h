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

//flashing octopuses

struct Task
{
	static inline Grid<10,10,int> startGrid;
	static constexpr int repeats = 100;
	static void run() {

		std::ifstream inputStream("2021/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		startGrid.fillFromStream(inputStream, [](char ch){return ch - '0';});

		Grid<10,10,int> grid = startGrid;
		int totalExplosions = 0;
		for (int rep = 0; rep < repeats; rep++) {
			bool newExplosions = false;
			grid.foreach([](int& val){val++;});
			Grid<10,10,bool> exploded;
			exploded.clear(false);
			do {
				newExplosions = false;
				for (int x = 0; x < grid.width; x++) {
					for (int y = 0; y < grid.height; y++) {
						if (grid.get(x,y) > 9) {
							if (exploded.get(x,y) == true) continue;
							newExplosions = true;
							totalExplosions++;
							exploded.set(x,y,true);
							for (auto [nx,ny] : grid.getNeighbourCoords8(x,y)) {
								grid.getRef(nx,ny)++;
							}
						}
					}
				}
			} while (newExplosions);

			grid.foreach([](int& val){if (val>9) val = 0;});
		}
		std::cout << totalExplosions << std::endl;
	}
	static void runPart2() {
		Grid<10,10,int> grid = startGrid;
		int rep = 0;
		while (true) {
			rep++;
			int explosions = 0;
			bool newExplosions = false;
			grid.foreach([](int& val){val++;});
			Grid<10,10,bool> exploded;
			exploded.clear(false);
			do {
				newExplosions = false;
				for (int x = 0; x < grid.width; x++) {
					for (int y = 0; y < grid.height; y++) {
						if (grid.get(x,y) > 9) {
							if (exploded.get(x,y) == true) continue;
							explosions++;
							newExplosions = true;
							exploded.set(x,y,true);
							for (auto [nx,ny] : grid.getNeighbourCoords8(x,y)) {
								grid.getRef(nx,ny)++;
							}
						}
					}
				}
			} while (newExplosions);

			if (explosions == 100) {
				std::cout << rep << std::endl;
				return;
			}
			grid.foreach([](int& val){if (val>9) val = 0;});
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/