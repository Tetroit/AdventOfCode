#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <queue>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "vec.h"
#include "utils.h"

//generating cave and pathfinding with switching tools

static constexpr int MAX_INT = std::numeric_limits<int>::max();
static constexpr int MIN_INT = std::numeric_limits<int>::min();

enum struct Equipment {
	NEITHER = 0,
	TORCH = 1,
	CLIMBING = 2
};
struct Node {
	int neither = MAX_INT;
	int torch = MAX_INT;
	int climbing = MAX_INT;
	int& operator[](Equipment equip) {
		switch (equip) {
			case Equipment::NEITHER: return neither;
			case Equipment::TORCH: return torch;
			case Equipment::CLIMBING: return climbing;
		}
	}
};
struct State {
	ivec2 pos;
	Equipment equip;
	int path;
	std::vector<State> Next() const {
		return std::vector<State>{
			{pos + ivec2{1, 0}, equip, path + 1},
			{pos + ivec2{0, 1}, equip, path + 1},
			{pos + ivec2{-1, 0}, equip, path + 1},
			{pos + ivec2{0, -1}, equip, path + 1},
			{pos, (Equipment)(((int)equip + 1) % 3), path + 7},
			{pos, (Equipment)(((int)equip + 2) % 3), path + 7}
		};
	}
};

struct StateComp {
	bool operator()(const State& lhs, const State& rhs) const {
		return lhs.path > rhs.path;
	}
};
struct Task
	{
		static constexpr int depth = 11541;
		static constexpr int targetX = 14;
		static constexpr int targetY = 778;
		static inline DynamicGrid<int> grid;
		static inline std::unordered_set<ivec2, ivec2hash> generated;
		static inline std::unordered_map<ivec2, Node, ivec2hash> weights;
		static void generateErosion (GridBase<int>& map, int x, int y) {
			int res = 0;
			if (x == 0 && y == 0) res = 0;
			else if (x == targetX && y == targetY) res = 0;
			else if (x == 0) res = y * 48271;
			else if (y == 0) res = x * 16807;
			else {
				if (!generated.contains({x-1,y})) generateErosion(grid, x-1, y);
				if (!generated.contains({x,y-1})) generateErosion(grid, x, y-1);
				res = map.get(x-1,y) * map.get(x, y-1);
			}
			res = (res + depth) % 20183;
			map.set(x, y, res);
			generated.insert({x,y});
		}
		static void generateErosionDynamic (DynamicGrid<int>& map, int x, int y) {
			map.expand(x + 1, y + 1);
			generateErosion(map, x, y);
		}
		static void run() {

			grid.resize(targetX+1, targetY+1);
			generateErosion(grid, targetX, targetY);
			generateErosion(grid, targetX-1, targetY); //this recurses
			generateErosion(grid, targetX, targetY-1); //this recurses
			int cnt = 0;
			grid.foreach([&cnt](int val){cnt += val % 3;});
			grid.print([](int val) {
				switch (val%3) {
					case 0: return '.';
					case 1: return '=';
					case 2: return '|';
				}
			});
			cnt -= grid.get(targetX, targetY) % 3;
			cnt -= grid.get(0, 0) % 3;
			std::cout << cnt << std::endl;
			//11859 high
		}
		static bool canGo (const DynamicGrid<int>& map, const State& state) {
			int cave = map.get(state.pos.x, state.pos.y) % 3;
			return cave != (int)state.equip;
		}
		static void runPart2() {
			State startState = State{{0,0},Equipment::TORCH, 0};
			std::priority_queue<State, std::vector<State>, StateComp> queue;
			queue.push(startState);
			weights[{0,0}] = {MAX_INT,0,MAX_INT};
			while (!queue.empty()) {
				auto current = queue.top();
				queue.pop();

				if (weights[current.pos][current.equip] < current.path) {
					continue;
				}
				if (current.pos.x == targetX && current.pos.y == targetY && current.equip == Equipment::TORCH) {
					std::cout << current.path << std::endl;
					return;
				}

				for (auto next : current.Next()) {
					auto [x,y] = next.pos;
					if (x < 0 || y < 0) continue;
					if (!generated.contains(next.pos)) {
						generateErosionDynamic(grid, x, y);
					}

					if (canGo(grid, next)) {
						if (weights[next.pos][next.equip] > next.path) {
							weights[next.pos][next.equip] = next.path;
							queue.push(next);
						}
					}
				}
			}
		}
	};

//-------------- NOTES AREA ----------------
/*

*/