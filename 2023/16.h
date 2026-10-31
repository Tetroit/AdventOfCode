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

//splitting and mirroring light beam

struct Task
{
	static inline const std::vector<ivec2> dirs{{1,0}, {0,-1}, {-1,0}, {0,1}};
	static inline const std::unordered_map<char, std::vector<int>> mirrors{
	{'/', {1,0,3,2}},
	{'\\', {3,2,1,0}}
	};
	static inline DynamicGrid<char> grid;
	static inline DynamicGrid<int> passes;
	static void traceBeam(int dir, ivec2 pos) {
		static auto tryRegDir = [](int d, ivec2 p) {
			int bit = (1 << d);
			int& cell = passes.getRef(p.x, p.y);
			if ((cell & bit) > 0) return false;
			cell |= bit;
			return true;
		};
		do {
			pos += dirs.at(dir);
			if (!grid.inRange(pos.x, pos.y))
				return;
			char cell = grid.get(pos.x, pos.y);
			if (cell == '\\' || cell == '/') {
				dir = mirrors.at(cell).at(dir);
				if (!tryRegDir(dir, pos)) break;
			}
			else if ((dir == 0 || dir == 2) && cell == '|') {
				if (tryRegDir(1, pos))
					traceBeam(1, pos);
				if (!tryRegDir(3, pos))
					break;
				dir = 3;
			}
			else if ((dir == 1 || dir == 3) && cell == '-') {
				if (tryRegDir(0, pos))
					traceBeam(0, pos);
				if (!tryRegDir(2, pos))
					break;
				dir = 2;
			}
			else {
				if (!tryRegDir(dir, pos))
					break;
			}

		} while (true);
	}
	static void run() {

		std::ifstream inputStream("2023/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		grid.fillFromStream(inputStream, DefaultConvert<char>());
		inputStream.close();
		passes.resize(grid.getWidth(), grid.getHeight());
		passes.clear(0);
		traceBeam(0, {-1, 0});
		std::cout << passes.count([](int val){return val > 0;}) << std::endl;

	}
	static int score(int x, int y, int dir) {
		traceBeam(dir, {x, y});
		int cnt = passes.count([](int val){return val > 0;});
		passes.clear(0);
		return cnt;
	}
	static void runPart2() {
		passes.clear(0);
		int max = 0;
		for (int i = 0; i < passes.getWidth(); ++i) {
			max = std::max(max, score(-1, i, 0));
			max = std::max(max, score(i, -1, 3));
			max = std::max(max, score(grid.getWidth(), i, 2));
			max = std::max(max, score(i, grid.getWidth(), 1));
		}
		std::cout << max << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/