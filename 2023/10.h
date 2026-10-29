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

//Finding tube loop

struct Task
{
	static inline DynamicGrid<char> map;
	static char& get(ivec2 coord) {
		return map.getRef(coord.x, coord.y);
	}
	static inline const std::vector<ivec2> facingVec ={
		{1,0},
		{0,-1},
		{-1,0},
		{0,1}
	};
	static inline const std::unordered_map<char, std::vector<bool>> canTravel{
		{'|', {0,1,0,1}},
		{'-', {1,0,1,0}},
		{'L', {1,1,0,0}},
		{'F', {1,0,0,1}},
		{'J', {0,1,1,0}},
		{'7', {0,0,1,1}},
	};
	static int nextDir(char c, int dir) {
		int back = (dir + 2) % 4;
		for (int i=0; i<4; i++) {
			if (canTravel.at(c)[i] && i != back) {
				return i;
			}
		}
		return -1;
	}
	static void run() {

		std::ifstream inputStream("2023/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		map.fillFromStream(inputStream, DefaultConvert<char>());
		inputStream.close();

		ivec2 start;
		map.foreachCoord([&start](int x, int y) {
			if (map.get(x,y) == 'S') {
				start = {x,y};
			}
		});
		int dir = 0;
		for (dir=0; dir<4; dir++) {
			int back = (dir + 2)%4;
			if (canTravel.at(get(start + facingVec[dir]))[back]) {
				break;
			}
		}

		ivec2 next = start + facingVec[dir];
		int loop = 1;
		do {
			dir = nextDir(get(next), dir);
			next += facingVec[dir];
			loop++;
		} while (next != start);
		std::cout << loop/2 << std::endl;
	}
	static void runPart2() {
		DynamicGrid<int> mask;
		mask.resize(map.getWidth(), map.getHeight());

		ivec2 start;
		int startDir = 0;
		map.foreachCoord([&start](int x, int y) {
			if (map.get(x,y) == 'S') {
				start = {x,y};
			}
		});
		int dir = 0;
		for (dir=0; dir<4; dir++) {
			int back = (dir + 2)%4;
			if (canTravel.at(get(start + facingVec[dir]))[back]) {
				break;
			}
		}
		startDir = dir;

		mask.set(start.x, start.y, 1);
		ivec2 next = start + facingVec[dir];

		int fullTurn = 0;
		while (next != start) {
			int turn = 0;
			mask.set(next.x, next.y, 1);
			turn -= dir;
			dir = nextDir(get(next), dir);
			turn += dir;
			turn = Utils::Loop(turn, -1, 2);
			fullTurn += turn;
			next += facingVec[dir];
		}
		bool turnRight = fullTurn < 0;
		next = start + facingVec[startDir];
		dir = startDir;
		while (next != start) {
			ivec2 d = facingVec[(dir + (turnRight ? 3 : 1)) % 4];
			ivec2 inside = next + d;
			//check both directions for concave turns
			while (mask.get(inside.x, inside.y) != 1) {
				mask.set(inside.x, inside.y, 2);
				inside += d;
			}
			dir = nextDir(get(next), dir);
			d = facingVec[(dir + (turnRight ? 3 : 1)) % 4];
			inside = next + d;
			//check both directions for concave turns
			while (mask.get(inside.x, inside.y) != 1) {
				mask.set(inside.x, inside.y, 2);
				inside += d;
			}
			next += facingVec[dir];
		}
		// mask.print([](int val){return char(val + '0');});
		std::cout << mask.count([](int val){return val == 2;}) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
*/