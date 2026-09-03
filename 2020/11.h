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

//occupying seats cellular automata

struct Task
{
	static inline DynamicGrid<char> init;
	static void run() {

		std::ifstream inputStream("2020/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		init.fillFromStream(inputStream, DefaultConvert<char>());
		inputStream.close();

		DynamicGrid<char> grid1 = init;
		DynamicGrid<char> grid2 = init;

		DynamicGrid<char>* src = &grid1;
		DynamicGrid<char>* dst = &grid2;

		while (true) {
			src->foreachCoord([&](int x, int y) {
				int occupied = src->countNeighbours8(x,y,[](char val) {return val == '#';});
				if (src->get(x,y) == 'L') {
					if (occupied== 0) {
						dst->set(x,y,'#');
						return;
					}
					dst->set(x,y,'L');
					return;
				}
				if (src->get(x,y) == '#') {
					if (occupied >= 4) {
						dst->set(x,y,'L');
						return;
					}
					dst->set(x,y,'#');
					return;
				}
				dst->set(x,y,'.');
			});
			std::swap(src,dst);
			bool same = true;
			for (int y = 0; y < grid1.getHeight(); y++) {
				for (int x = 0; x < grid1.getWidth(); x++) {
					if (src->get(x,y) != dst->get(x,y)) {
						same = false;
					}
				}
			}
			if (same) {
				std::cout << src->count([](char val) {return val == '#';}) << std::endl;
				break;
			}
		}
	}
	static void runPart2() {
		DynamicGrid<char> grid1 = init;
		DynamicGrid<char> grid2 = init;

		DynamicGrid<char>* src = &grid1;
		DynamicGrid<char>* dst = &grid2;

		while (true) {
			src->foreachCoord([&](int x, int y) {
				int visible = 0;
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (dx == 0 && dy == 0) continue;
						auto raycastRes = src->raycast(x+dx,y+dy,dx,dy,[](char val){return val == '#' || val == 'L';});
						if (raycastRes.has_value()) {
							auto& [rayx, rayy] = raycastRes.value();
							if (src->get(rayx,rayy) == '#') visible++;
						}
					}
				}
				if (src->get(x,y) == 'L') {
					if (visible == 0) {
						dst->set(x,y,'#');
						return;
					}
					dst->set(x,y,'L');
					return;
				}
				if (src->get(x,y) == '#') {
					if (visible >= 5) {
						dst->set(x,y,'L');
						return;
					}
					dst->set(x,y,'#');
					return;
				}
				dst->set(x,y,'.');
			});
			std::swap(src,dst);
			bool same = true;
			for (int y = 0; y < grid1.getHeight(); y++) {
				for (int x = 0; x < grid1.getWidth(); x++) {
					if (src->get(x,y) != dst->get(x,y)) {
						same = false;
					}
				}
			}
			if (same) {
				std::cout << src->count([](char val) {return val == '#';}) << std::endl;
				break;
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/