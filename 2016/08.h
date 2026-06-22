#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "GridBase.h"
#include "utils.h"

//rotating screen pixels
struct Task
{
	static constexpr int width = 50;
	static constexpr int height = 6;
	static inline Grid<50, 6, uint8_t> grid;
	static void run() {

		std::ifstream inputStream("2016/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		grid.clear(0);
		std::string line;
		while (inputStream >> line)
		{
			if (line == "rect") {\
				int newWidth, newHeight;
				char _x;
				inputStream >> newWidth >> _x >> newHeight;
				for (int x = 0; x < newWidth; x++) {
					for (int y = 0; y < newHeight; y++) {
						grid.set(x, y, 1);
					}
				}
			}
			if (line == "rotate") {
				char shit;
				std::string dim;
				inputStream >> dim;
				if (dim == "row") {
					int y, amount;
					inputStream >> shit >> shit >> y >> shit >> shit >> amount;

					std::array<uint8_t, width> projected{};
					for (int i=0; i<width; i++) {
						projected[(i + amount)%width] = grid.get(i, y);
					}
					for (int i=0; i<width; i++) {
						grid.set(i, y, projected[i]);
					}
				}
				if (dim == "column") {
					int x, amount;
					inputStream >> shit >> shit >> x >> shit >> shit >> amount;

					std::array<uint8_t, height> projected{};
					for (int i=0; i<height; i++) {
						projected[(i + amount)%height] = grid.get(x, i);
					}
					for (int i=0; i<height; i++) {
						grid.set(x, i, projected[i]);
					}
				}
			}
		}
		inputStream.close();
		int cnt = 0;
		grid.foreach([&cnt](const uint8_t& val) {
			if (val == 1)
				cnt++;
		});
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		grid.print([](uint8_t x) {
			if (x == 1) return '#';
			return '.';
		});
	}
};

//-------------- NOTES AREA ----------------
/*

*/