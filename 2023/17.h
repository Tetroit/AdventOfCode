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

//dijkstra in single line


struct Task
{
	static inline const std::vector<ivec2> dirs{{1,0}, {0,-1}, {-1,0}, {0,1}};
	struct Node {
		int x, y, w, dir, n;
		friend bool operator> (const Node& n1, const Node& n2) {
			return n1.w > n2.w;
		}
	};
	template <int size>
	struct CellInfo {
		std::array<std::array<int,4>, size> data;
		explicit CellInfo() {
			for (int i = 0; i < size; ++i) {
				data[i].fill(INT_MAX);
			}
		}
		bool update(int dir, int n, int val) {
			int& dst = data[n][dir];
			if (dst > val) {
				dst = val;
				return true;
			}
			return false;
		}
		int getMin() {
			int min = INT_MAX;
			for (int i=0; i<size; ++i) {
				for (int j=0; j<4; ++j) {
					if (min > data[i][j]) min = data[i][j];
				}
			}
			return min;
		}
	};
	static inline DynamicGrid<int> grid;
	static void run() {

		std::ifstream inputStream("2023/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		grid.fillFromStream(inputStream, [](char ch){return int(ch - '0');});
		inputStream.close();

		DynamicGrid<CellInfo<3>> weights;

		weights.resize(grid.getWidth(), grid.getHeight());
		weights.clear(CellInfo<3>());
		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
		queue.push({0, 0, 0, 0, 0});
		while (!queue.empty()) {
			auto node = queue.top();
			if (node.x == grid.getWidth() - 1 && node.y == grid.getHeight() - 1) {
				std::cout << weights.get(node.x, node.y).getMin() << std::endl;
				queue = {};
				break;
			}
			queue.pop();
			for (int dir = 0; dir < 4; dir++) {
				ivec2 next = ivec2(node.x, node.y) + dirs.at(dir);
				if ((dir + 2) % 4 == node.dir) continue;
				if (!grid.inRange(next.x, next.y)) continue;

				int newW = node.w + grid.get(next.x, next.y);
				if (node.dir == dir) {
					if (node.n == 2) continue;
					if (weights.getRef(next.x, next.y).update(dir, node.n + 1, newW))
						queue.push({next.x, next.y, newW, dir, node.n + 1});
					continue;
				}
				if (weights.getRef(next.x, next.y).update(dir, 0, newW))
				queue.push({next.x, next.y, newW, dir, 0});
			}
		}
	}
	static void runPart2() {
		DynamicGrid<CellInfo<10>> weights;

		weights.resize(grid.getWidth(), grid.getHeight());
		weights.clear(CellInfo<10>());
		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
		queue.push({0, 0, 0, 0, 0});
		while (!queue.empty()) {
			auto node = queue.top();
			if (node.x == grid.getWidth() - 1 && node.y == grid.getHeight() - 1) {
				std::cout << weights.get(node.x, node.y).getMin() << std::endl;
				queue = {};
				break;
			}
			queue.pop();
			for (int dir = 0; dir < 4; dir++) {

				if ((dir + 2) % 4 == node.dir) continue;

				if (node.dir == dir) {

					ivec2 next = ivec2(node.x, node.y) + dirs.at(dir);

					if ((dir + 2) % 4 == node.dir) continue;
					if (!grid.inRange(next.x, next.y)) continue;

					int newW = node.w + grid.get(next.x, next.y);

					if (node.n == 9) continue;
					if (weights.getRef(next.x, next.y).update(dir, node.n + 1, newW))
						queue.push({next.x, next.y, newW, dir, node.n + 1});
					continue;
				}

				ivec2 next = ivec2(node.x, node.y) + dirs.at(dir) * 4;

				if (!grid.inRange(next.x, next.y)) continue;

				int newW = node.w;
				for (int i=1; i<=4; i++) {
					ivec2 step = ivec2(node.x, node.y) + dirs.at(dir) * i;
					newW += grid.get(step.x, step.y);
				}

				if (weights.getRef(next.x, next.y).update(dir, 3, newW))
					queue.push({next.x, next.y, newW, dir, 3});
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/