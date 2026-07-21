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

//overlapping rectangles


struct Rect {
	int id;
	int x, y;
	int w, h;
	[[nodiscard]] int right() const{return x+w-1;}
	[[nodiscard]] int left() const{return x;}
	[[nodiscard]] int bottom() const{return y+h-1;}
	[[nodiscard]] int top() const{return y;}
	bool overlaps(const Rect& other) {
		if (right() < other.left() || other.right() < left()) return false;
		if (bottom() < other.top() || other.bottom() < top()) return false;
		return true;
	}
};
struct Task
{

	static inline std::vector<Rect> rects;
	static inline Grid<1000, 1000, int> grid;
	static void run() {

		std::ifstream inputStream("2018/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char shit;
		while (inputStream >> shit)
		{
			int id;
			Rect rect{};
			inputStream >> rect.id;
			inputStream.ignore(3);
			inputStream >> rect.x >> shit >> rect.y >> shit >> rect.w >> shit >> rect.h;
			rects.push_back(rect);
		}
		inputStream.close();

		for (const auto& rect : rects) {
			for (int x=rect.x; x<rect.x+rect.w; x++) {
				for (int y=rect.y; y<rect.y+rect.h; y++) {
					grid.getRef(x,y)++;
				}
			}
		}
		int cnt = 0;
		grid.foreach([&cnt](int val) {
			if (val >= 2) {
				cnt++;
			}
		});
		std::cout << cnt << std::endl;
		//6392108
	}
	static void runPart2() {
		for (int i=0; i<rects.size(); i++) {
			bool clear = true;
			for (int j = 0; j<rects.size(); j++) {
				if (i == j) continue;
				if (rects[i].overlaps(rects[j])) {
					clear = false;
				}
			}
			if (clear) {
				std::cout << rects[i].id << std::endl;
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/