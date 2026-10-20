#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <queue>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

using Node = std::pair<int, ivec2>;
//moving among blizzards
struct Task
{
	static inline const std::array<ivec2, 4> facing {{
		{1,0},
		{0,-1},
		{-1,0},
		{0,1}
	}};
	struct CompareGreater {
		bool operator()(const Node& l, const Node& r) const {
			if (l.first != r.first) return l.first > r.first;
			if (l.second.x != r.second.x) return l.second.x > r.second.x;
			return l.second.y > r.second.y;
		}
	};
	static inline DynamicGrid<int> initMap;
	static void advance(DynamicGrid<int>* src, DynamicGrid<int>* dst) {
		dst->clear(0);
		src->foreachCoord([&](int x, int y) {
			if (x == 0 || y == 0 || x == src->getWidth()-1 || y == src->getHeight()-1) {
				dst->set(x, y, src->get(x, y));
				return;
			}
			ivec2 pos = ivec2(x, y);
			int& dstCell = dst->getRef(x,y);
			for (int i=0; i<4; i++) {
				int oppositeDir = (i + 2) % 4;
				int odMask = 1 << oppositeDir;
				ivec2 nextPos = pos + facing[i];
				int neigh = src->get(nextPos.x, nextPos.y);
				if (neigh == -1) {
					if (nextPos.x == 0) nextPos.x = src->getWidth() - 2;
					if (nextPos.y == 0) nextPos.y = src->getHeight() - 2;
					if (nextPos.x == src->getWidth() - 1) nextPos.x = 1;
					if (nextPos.y == src->getHeight() - 1) nextPos.y = 1;
				}
				neigh = src->get(nextPos.x, nextPos.y);
				if (neigh & odMask) {
					dstCell |= odMask;
				}
			}
		});
	}
	static void run() {

		std::ifstream inputStream("2022/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		initMap.fillFromStream(inputStream, [](char ch) {
			switch (ch) {
				case '#': return -1;
				case '.': return 0;
				case '>': return 1;
				case '^': return 2;
				case '<': return 4;
				case 'v': return 8;
				default: return -2;
			}
		});
		inputStream.close();
		auto printFunc = [](int cell) {
			if (cell == 0) return '.';
			if (cell == -1) return '#';
			int cnt = 0;
			int dir = -1;
			for (int i=0; i<4; i++) {
				if (cell & (1 << i)) {
					cnt++;
					dir = i;
				}
			}
			if (cnt > 1) return char(cnt + '0');
			if (dir == 0) return '>';
			if (dir == 1) return '^';
			if (dir == 2) return '<';
			if (dir == 3) return 'v';
			return '?';
		};
		std::priority_queue<
		Node,
		std::vector<Node>,
		CompareGreater> queue;
		queue.emplace(0, ivec2(1,0));
		DynamicGrid<int> map1 = initMap;
		DynamicGrid<int> map2 = initMap;
		auto src = &map1;
		auto dst = &map2;

		DynamicGrid<uint8_t> visited;
		visited.resize(src->getWidth(), src->getHeight());
		int step = 0;
		do {
			Node el = queue.top();
			queue.pop();
			if (el.first == step) {
				advance(src, dst);
				std::swap(src, dst);
				step++;
				visited.clear(0);
				// src->print(printFunc);
			}
			if (src->get(el.second.x, el.second.y) == 0 && visited.get(el.second.x, el.second.y) == 0) {
				queue.emplace(step, el.second);
				visited.set(el.second.x, el.second.y, 1);
			}
			for (int i=0; i<4; i++) {
				ivec2 nextPos = el.second + facing[i];
				if (!src->inRange(nextPos.x, nextPos.y)) continue;
				if (nextPos.x == src->getWidth()-2 && nextPos.y == src->getHeight()-1) {
					std::cout << step << "\n";
					return;
				}
				if (src->get(nextPos.x, nextPos.y) == 0) {
					if (visited.get(nextPos.x, nextPos.y) == 1) continue;
					queue.emplace(step, nextPos);
					visited.set(nextPos.x, nextPos.y, 1);
				}
			}
		} while (!queue.empty());
	}
	static void runPart2() {
		auto printFunc = [](int cell) {
			if (cell == 0) return '.';
			if (cell == -1) return '#';
			int cnt = 0;
			int dir = -1;
			for (int i=0; i<4; i++) {
				if (cell & (1 << i)) {
					cnt++;
					dir = i;
				}
			}
			if (cnt > 1) return char(cnt + '0');
			if (dir == 0) return '>';
			if (dir == 1) return '^';
			if (dir == 2) return '<';
			if (dir == 3) return 'v';
			return '?';
		};
		std::priority_queue<
		Node,
		std::vector<Node>,
		CompareGreater> queue;
		queue.emplace(0, ivec2(1,0));
		DynamicGrid<int> map1 = initMap;
		DynamicGrid<int> map2 = initMap;
		auto src = &map1;
		auto dst = &map2;

		DynamicGrid<uint8_t> visited;
		visited.resize(src->getWidth(), src->getHeight());
		int step = 0;
		int checkpointID = 0;
		std::vector<ivec2> checkpoints{
				{src->getWidth()-2, src->getHeight()-1},
				{1,0},
				{src->getWidth()-2, src->getHeight()-1}
		};
		do {
			Node el = queue.top();
			queue.pop();
			if (el.first == step) {
				advance(src, dst);
				std::swap(src, dst);
				step++;
				visited.clear(0);
				// src->print(printFunc);
			}
			if (src->get(el.second.x, el.second.y) == 0 && visited.get(el.second.x, el.second.y) == 0) {
				queue.emplace(step, el.second);
				visited.set(el.second.x, el.second.y, 1);
			}
			bool reachedCheckpoint = false;
			for (int i=0; i<4; i++) {
				ivec2 nextPos = el.second + facing[i];
				if (!src->inRange(nextPos.x, nextPos.y)) continue;

				if (src->get(nextPos.x, nextPos.y) == 0) {
					if (nextPos == checkpoints[checkpointID]) {
						reachedCheckpoint = true;
						checkpointID++;
						if (checkpointID == checkpoints.size()) {
							std::cout << step << "\n";
							return;
						}
						queue = {};
						queue.emplace(step, nextPos);
						break;
					}
					if (visited.get(nextPos.x, nextPos.y) == 1) continue;
					queue.emplace(step, nextPos);
					visited.set(nextPos.x, nextPos.y, 1);
				}
			}
		} while (!queue.empty());
	}
};

//-------------- NOTES AREA ----------------
/*

*/