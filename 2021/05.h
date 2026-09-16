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
#include "utils.h"
#include "vec.h"

//plotting lines on screen

struct Task
{
	static inline std::regex rectPattern = std::regex(R"((\d+),(\d+) -> (\d+),(\d+))");
	static inline std::vector<std::array<int, 4>> rects;
	static void run() {

		std::ifstream inputStream("2021/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_match(line, match, rectPattern);
			int x1 = std::stoi(match[1]);
			int y1 = std::stoi(match[2]);
			int x2 = std::stoi(match[3]);
			int y2 = std::stoi(match[4]);
			rects.emplace_back(std::array<int, 4>{x1,y1, x2,y2});
		}
		inputStream.close();
		std::unordered_map<ivec2, int, ivec2hash> map;
		for (auto& rect : rects) {

			int minX = std::min(rect[0], rect[2]);
			int maxX = std::max(rect[0], rect[2]);
			int minY = std::min(rect[1], rect[3]);
			int maxY = std::max(rect[1], rect[3]);
			if (rect[0] == rect[2]) {
				for (int y=minY; y<=maxY; y++) {
					map[{minX,y}]++;
				}
			}
			else if (rect[1] == rect[3]) {
				for (int x=minX; x<=maxX; x++) {
					map[{x,minY}]++;
				}
			}
		}
		int overlaps = 0;
		for (auto& [pos, cnt] : map) {
			if (cnt >= 2) overlaps++;
		}
		std::cout << overlaps << std::endl;
	}
	static void runPart2() {
		std::unordered_map<ivec2, int, ivec2hash> map;
		for (auto& rect : rects) {
			int minX = std::min(rect[0], rect[2]);
			int maxX = std::max(rect[0], rect[2]);
			int minY = std::min(rect[1], rect[3]);
			int maxY = std::max(rect[1], rect[3]);
			if (rect[0] == rect[2]) {
				for (int y=minY; y<=maxY; y++) {
					map[{minX,y}]++;
				}
				continue;
			}
			if (rect[1] == rect[3]) {
				for (int x=minX; x<=maxX; x++) {
					map[{x,minY}]++;
				}
				continue;
			}
			int diff = abs( rect[2] - rect[0]);
			if (rect[2] - rect[0] == rect[3] - rect[1]) {
				for (int x = 0; x<=diff; x++) {
					map[{minX + x, minY + x}]++;
				}
				continue;
			}
			if (rect[2] - rect[0] == rect[1] - rect[3]) {
				for (int x = 0; x<=diff; x++) {
					map[{minX + x, maxY - x}]++;
				}
			}
		}
		int overlaps = 0;
		for (auto& [pos, cnt] : map) {
			if (cnt >= 2) overlaps++;
		}
		std::cout << overlaps << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/