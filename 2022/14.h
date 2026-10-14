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

//sand simulation

struct Task
{
	static inline DynamicGrid<int> grid;
	static ivec2 parseVec2(std::string str) {
		int num = 0;
		int id = 0;
		bool prevNum = false;
		ivec2 res = {0, 0};
		for (char i : str) {
			if (std::isdigit(i)) {
				num = num * 10 + (i - '0');
				prevNum = true;
			}
			else if (prevNum) {
				res[id] = num;
				num = 0;
				id++;
				prevNum = false;
			}
		}
		res[id] = num;
		return res;
	}
	static void run() {

		std::ifstream inputStream("2022/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		grid.resize(1000, 10);
		std::string line;
		while (std::getline(inputStream, line))
		{
			auto words = Utils::split(line," -> ");
			ivec2 start = parseVec2(words[0]);
			if (start.y >= grid.getHeight()) grid.resize(grid.getWidth(), start.y+1);

			for (int i=1; i<words.size(); i++) {
				ivec2 next = parseVec2(words[i]);
				if (next.y >= grid.getHeight()) grid.resize(grid.getWidth(), next.y+1);
				if (start.x == next.x) {

					int min = std::min(start.y, next.y);
					int max = std::max(start.y, next.y);
					for (int y = min; y<=max; y++) {
						grid.set(start.x, y, 1);
					}
				}
				if (start.y == next.y) {
					int min = std::min(start.x, next.x);
					int max = std::max(start.x, next.x);
					for (int x = min; x<=max; x++) {
						grid.set(x, start.y, 1);
					}
				}
				start = next;
			}
		}
		inputStream.close();

		while (true) {
			ivec2 sand = {500, 0};
			bool abyss = false;
			while (true) {
				if (sand.y == grid.getHeight() - 1) {
					abyss = true;
					break;
				}
				if (grid.get(sand.x, sand.y + 1) == 0) sand.y++;
				else if (grid.get(sand.x-1, sand.y + 1) == 0) {
					sand.x--;
					sand.y++;
				}
				else if (grid.get(sand.x+1, sand.y + 1) == 0) {
					sand.x++;
					sand.y++;
				}
				else {
					grid.set(sand.x, sand.y, 2);
					break;
				}
			}
			if (abyss) break;
		}

		// grid.print([](char ch){return ch == 1 ? '#' : ch == 2 ? 'o' : '.';});

		std::cout << grid.count([](int val) {
			return val == 2;
		}) << std::endl;
	}
	static void runPart2() {
		grid.resize(1000, grid.getHeight() + 2);
		for (int i=0; i<grid.getWidth(); i++) {
			grid.set(i, grid.getHeight() - 1, 1);
		}
		while (true) {
			ivec2 sand = {500, 0};
			bool stuck = false;
			while (true) {
				if (grid.get(sand.x, sand.y + 1) == 0) sand.y++;
				else if (grid.get(sand.x-1, sand.y + 1) == 0) {
					sand.x--;
					sand.y++;
				}
				else if (grid.get(sand.x+1, sand.y + 1) == 0) {
					sand.x++;
					sand.y++;
				}
				else {
					if (sand.y == 0) stuck = true;
					grid.set(sand.x, sand.y, 2);
					break;
				}
			}
			if (stuck) break;
		}

		grid.print([](char ch){return ch == 1 ? '#' : ch == 2 ? 'o' : '.';});

		std::cout << grid.count([](int val) {
			return val == 2;
		}) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/