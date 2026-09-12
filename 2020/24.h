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

// flipping hexagonal tiles and cellular automata

struct Task
{
	static inline std::unordered_map<ivec2, bool, ivec2hash> map;
	static void run() {

		std::ifstream inputStream("2020/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			ivec2 pos = {0,0};
			for (int i=0; i<line.size(); i++) {
				if (line[i] == 'e') pos.x++;
				if (line[i] == 'w') pos.x--;
				if (line[i] == 'n') {
					i++;
					if (line[i] == 'e') pos.y++;
					if (line[i] == 'w') {pos.y++; pos.x--;}
				}
				if (line[i] == 's') {
					i++;
					if (line[i] == 'e') {pos.y--; pos.x++;}
					if (line[i] == 'w') pos.y--;
				}
			}
			map[pos] = !map[pos];
		}
		inputStream.close();
		int blackCnt = 0;
		for (auto [pos, black] : map) {
			if (black) blackCnt++;
		}
		std::cout << blackCnt << std::endl;
	}
	static int countNeighbours(ivec2 pos, const std::unordered_map<ivec2, bool, ivec2hash>& map) {
		int x = pos.x;
		int y = pos.y;
		int cnt = 0;
		auto check = [&](ivec2 posCheck) {
			if (!map.contains(posCheck)) return false;
			return map.at(posCheck);
		};
		if (check({x-1,y})) cnt++;
		if (check({x+1,y})) cnt++;
		if (check({x,y-1})) cnt++;
		if (check({x,y+1})) cnt++;
		if (check({x+1,y-1})) cnt++;
		if (check({x-1,y+1})) cnt++;
		return cnt;
	}
	static void runPart2() {
		std::unordered_map<ivec2, bool, ivec2hash> map2;
		auto* src = &map;
		auto* dst = &map2;
		for (int i=0; i<100; i++) {
			dst->clear();
			for (auto [pos, black] : *src) {
				if (!black) continue;
				int x = pos.x;
				int y = pos.y;
				src->operator[]({x+1,y});
				src->operator[]({x-1,y});
				src->operator[]({x,y-1});
				src->operator[]({x,y+1});
				src->operator[]({x+1,y-1});
				src->operator[]({x-1,y+1});
			}
			for (auto [pos, black] : *src) {
				int blackCnt = countNeighbours(pos, *src);
				if (src->at(pos))
					dst->operator[](pos) = blackCnt == 1 || blackCnt == 2;
				else
					dst->operator[](pos) = blackCnt == 2;
			}
			std::swap(src, dst);

		}
		int cnt = 0;
		for (auto [pos, black] : *src) {
			if (black) cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/