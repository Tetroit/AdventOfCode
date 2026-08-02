#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"

//water simulation
struct Task {
	struct Wall {
		int startX, endX;
		int startY, endY;
	};
	static inline DynamicGrid<uint8_t> map;
	static int reachBottom(DynamicGrid<uint8_t>& map, int x, int y) {
		bool firstIt = true;
		while (y < map.getHeight()) {
			if (map.get(x,y) == 3 && !firstIt) return -1;
 			if (map.get(x,y) == 2 || map.get(x,y) == 1) return y-1;
			map.set(x,y,3);
			y++;
			if (firstIt) firstIt = false;
		}
		return -1;
	}
	static bool hasBottom(DynamicGrid<uint8_t>& map, int x, int y) {
		if (y==map.getHeight()-1) return false;
		return map.get(x,y+1) == 2 || map.get(x,y+1) == 1;
	}
	static std::vector<int> fillRow(DynamicGrid<uint8_t>& map, int x, int y) {
		int left = x;
		int right = x;
		bool closedLeft = false;
		bool closedRight = false;
		while (left >= 0 && hasBottom(map,left,y)) {
			if (map.get(left,y) == 1) {
				closedLeft = true;
				break;
			}
			left--;
		};
		while (right < map.getWidth() && hasBottom(map,right,y)) {
			if (map.get(right,y) == 1) {
				closedRight = true;
				break;
			}
			right++;
		}
		if (closedLeft && closedRight) {
			for (int i=left+1; i<right; i++) {
				map.set(i,y,2);
			}
		}
		else {
			if (closedLeft) left++;
			if (closedRight) right--;
			for (int i = std::max(0, left); i <= std::min(map.getWidth()-1, right); i++) {
				map.set(i,y,3);
			}
		}
		std::vector<int> result;
		if (!closedLeft && left >= 0) result.push_back(left);
		if (!closedRight && right < map.getWidth()) result.push_back(right);
		return result;
	}
	static bool fillHollow(DynamicGrid<uint8_t>& map, int srcX, int srcY) {
		int toFill = reachBottom(map,srcX,srcY);
		if (toFill == -1) return false;
		std::vector<int> overflow;
		for (int y=toFill; y>srcY; y--) {
			overflow = fillRow(map,srcX,y);
			if (overflow.empty()) continue;

			bool fillable = true;
			for (const auto& stream : overflow) {
				if (stream < 0 || stream >= map.getWidth()) {
					fillable = false;
					continue;
				}
				if (!fillHollow(map,stream,y)) fillable = false;
			}
			if (!fillable)
				return false;
			else
				y++;
		}
		return true;
	}
	static void run() {

		std::ifstream inputStream("2018/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		std::string line;
		char coord;
		char shit;
		int depth;
		int start, end;
		int minX = std::numeric_limits<int>::max();
		int maxX = std::numeric_limits<int>::min();
		int minY = std::numeric_limits<int>::max();
		int maxY = std::numeric_limits<int>::min();
		std::vector<Wall> walls;
		while (std::getline(inputStream, line))
		{
			auto ss = std::istringstream(line);
			//  x      =     x1     ,     y     =     y1     .     .     y2
			ss>>coord>>shit>>depth>>shit>>shit>>shit>>start>>shit>>shit>>end;
			Wall wall;
			if (coord == 'x') {
				wall.startX = depth;
				wall.endX = depth;
				wall.startY = start;
				wall.endY = end;
			}
			else if (coord == 'y') {
				wall.startY = depth;
				wall.endY = depth;
				wall.startX = start;
				wall.endX = end;
			}
			minX = std::min(wall.startX, minX);
			minY = std::min(wall.startY, minY);
			maxX = std::max(wall.endX, maxX);
			maxY = std::max(wall.endY, maxY);
			walls.emplace_back(wall);
		}
		inputStream.close();
		minX--;
		maxX++;
		map.resize(maxX - minX + 1, maxY - minY + 1);
		map.clear(0);
		int waterStartLocal = 500 - minX;
		for (auto& wall : walls) {
			for (int x = wall.startX - minX; x <= wall.endX - minX; x++) {
				for (int y = wall.startY - minY; y <= wall.endY - minY; y++) {
					map.set(x,y,1);
				}
			}
		}
		map.set(waterStartLocal,0,3);
		fillHollow(map, waterStartLocal, 0);
		map.print([](uint8_t ch) {
			if (ch == 1) return '#';
			if (ch == 2) return '~';
			if (ch == 3) return '|';
			return '.';
		});

		std::cout << map.count([](uint8_t ch) { return ch == 2 || ch == 3; }) << std::endl;
	}
	static void runPart2() {

		std::cout << map.count([](uint8_t ch) { return ch == 2;}) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/