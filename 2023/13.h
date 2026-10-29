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

//looking for mirrored image

struct Task
{
	static inline std::vector<DynamicGrid<char>> grids;
	static inline std::vector<std::pair<int, int>> firstMirrors;

	static std::pair<int, int> getMirror(const DynamicGrid<char>& grid) {
		for (int x=0; x<grid.getWidth()-1; x++) {
			int left = x + 1;
			int right = grid.getWidth() - left;
			bool matches = true;
			for (int d = 0; d<std::min(left, right); d++) {
				for (int y=0; y<grid.getHeight(); y++) {
					if (grid.get(x - d, y) != grid.get(x + d + 1, y)) {
						matches = false;
						break;
					}
				}
				if (!matches) break;
			}
			if (!matches) continue;
			return {0, left};
		}
		for (int y=0; y<grid.getHeight() - 1; y++) {
			int up = y + 1;
			int down = grid.getHeight() - up;
			bool matches = true;
			for (int d = 0; d<std::min(up, down); d++) {
				for (int x=0; x<grid.getWidth(); x++) {
					if (grid.get(x, y - d) != grid.get(x, y + d + 1)) {
						matches = false;
						break;
					}
				}
				if (!matches) break;
			}
			if (!matches) continue;
			return {1, up};
		}
		return {-1, 0};
	}
	static std::pair<int, int> getSecondMirror(const DynamicGrid<char>& grid, int firstOrient, int firstPos) {
		for (int x=0; x<grid.getWidth()-1; x++) {
			if (firstOrient == 0 && x == firstPos) {
				continue;
			}
			int left = x + 1;
			int right = grid.getWidth() - left;
			int mismatches = 0;
			for (int d = 0; d<std::min(left, right); d++) {
				for (int y=0; y<grid.getHeight(); y++) {
					if (grid.get(x - d, y) != grid.get(x + d + 1, y)) {
						mismatches++;
						if (mismatches > 1)
							break;
					}
				}
				if (mismatches > 1) break;
			}
			if (mismatches > 1) continue;
			return {0, left};
		}
		for (int y=0; y<grid.getHeight() - 1; y++) {
			if (firstOrient == 1 && y == firstPos) {
				continue;
			}
			int up = y + 1;
			int down = grid.getHeight() - up;

			int mismatches = 0;
			for (int d = 0; d<std::min(up, down); d++) {
				for (int x=0; x<grid.getWidth(); x++) {
					if (grid.get(x, y - d) != grid.get(x, y + d + 1)) {
						mismatches++;
						if (mismatches > 1)
							break;
					}
				}
				if (mismatches > 1) break;
			}
			if (mismatches > 1) continue;
			return {1, up};
		}
		return {-1, 0};
	}
	static void run() {

		std::ifstream inputStream("2023/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int inY = 0;
		grids.emplace_back();
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				inY = 0;
				grids.emplace_back();
				continue;
			}
			grids.back().fillFromLine(inY++, line, DefaultConvert<char>());
		}
		inputStream.close();

		std::array<int, 2> axii{0,0};
		for (auto& grid : grids) {
			auto [orient, pos] = getMirror(grid);
			firstMirrors.emplace_back(orient, pos-1);
			if (orient >= 0)
				axii[orient] += pos;
		}
		std::cout << axii[0] + axii[1] * 100 << std::endl;
	}
	static void runPart2() {
		std::array<int, 2> axii{0,0};
		for (int i=0; i<grids.size(); i++) {
			auto& grid = grids[i];
			auto [orient, pos] = getSecondMirror(grid, firstMirrors.at(i).first, firstMirrors.at(i).second);
			firstMirrors.emplace_back(orient, pos);
			if (orient >= 0)
				axii[orient] += pos;
		}
		std::cout << axii[0] + axii[1] * 100 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/