#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <queue>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//backrooms simulator

struct Task
{
	static constexpr int fav = 1362;
	template<typename T>
	using grid = std::unordered_map<int, std::unordered_map<int, T>>;
	static inline grid<int8_t> cache;
	static inline grid<uint8_t> weights;
	static bool isWall(int x, int y, int add) {
		int code = x*x + 3*x + 2*x*y + y + y*y + add;
		int cnt = 0;
		while (code != 0) {
			if (code & 1) cnt++;
			code >>= 1;
		}
		return cnt & 1;
	}

	static int dijkstra(int startX, int startY, int endX, int endY) {
		weights.clear();
		weights[startX][startY] = 0;
		int shortest = -1;
		std::deque<std::tuple<int, int, uint8_t>> q;
		q.emplace_back(startX, startY, 0);
		while (!q.empty()) {
			bool found = false;
			auto [x0, y0, w0] = q.front();
			auto weight = weights[x0][y0];
			q.pop_front();
			std::vector<std::pair<int, int>> neighbours{
				{x0 + 1, y0},
				{x0 - 1, y0},
				{x0, y0 + 1},
				{x0, y0 - 1}
			};
			auto newWeight = weight+1;
			for (auto [x, y] : neighbours) {
				bool wall = false;
				if (x < 0 || y < 0)
					continue;
				if (cache[x][y] == -1)
					wall = true;
				if (cache[x][y] == 0) {
					wall = isWall(x,y,fav);
					cache[x][y] = wall ? -1 : 1;
					weights[x][y] = std::numeric_limits<uint8_t>::max();
				}
				if (wall) {
					continue;
				}
				if (weights[x][y] < newWeight) {
					continue;
				}
				weights[x][y] = newWeight;
				q.emplace_back(x, y, newWeight);
				if (x == endX && y == endY) {
					shortest = newWeight;
					found = true;
				}
			}
			if (found)
				return shortest;
		}
		return -1;
	}
	static int dijkstraCount(int startX, int startY, int rad) {
		weights.clear();
		weights[startX][startY] = 0;
		int cnt = 1;
		std::deque<std::tuple<int, int, uint8_t>> q;
		q.emplace_back(startX, startY, 0);
		while (!q.empty()) {
			bool found = false;
			auto [x0, y0, w0] = q.front();
			auto weight = weights[x0][y0];
			q.pop_front();
			std::vector<std::pair<int, int>> neighbours{
					{x0 + 1, y0},
					{x0 - 1, y0},
					{x0, y0 + 1},
					{x0, y0 - 1}
			};
			auto newWeight = weight+1;
			for (auto [x, y] : neighbours) {
				bool wall = false;
				bool newCell = false;
				if (x < 0 || y < 0)
					continue;
				if (cache[x][y] == -1)
					wall = true;
				if (cache[x][y] == 0) {
					wall = isWall(x,y,fav);
					cache[x][y] = wall ? -1 : 1;
					weights[x][y] = std::numeric_limits<uint8_t>::max();
					newCell = true;
				}
				if (wall) {
					continue;
				}
				if (weights[x][y] < newWeight) {
					continue;
				}
				if (newCell && newWeight <= rad) {
					q.emplace_back(x, y, newWeight);
					cnt++;
				}
				weights[x][y] = newWeight;
			}
		}
		return cnt;
	}
	static void run() {
		int path = dijkstra(1,1,31,39);
		std::cout << path << std::endl;
	}
	static void runPart2() {
		cache.clear();
		cache[1][1] = 1;
		int path = dijkstraCount(1,1,50);
		std::cout << path << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/