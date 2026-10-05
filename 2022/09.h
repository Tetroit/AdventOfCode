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

//moving tail to the head

struct Task
{
	static inline std::vector<std::pair<char, int>> dirs;
	static void follow(const ivec2& h, ivec2& t) {
		ivec2 d = h - t;
		ivec2 m;
		if (d.len() == 3 && d.x != 0 && d.y != 0) {
			m = {d.x < 0 ? -1 : 1,
				d.y < 0 ? -1 : 1
			};
		}
		else {
			m = {
				abs(d.x) > 1 ? (d.x < 0 ? -1 : 1) : 0,
				abs(d.y) > 1 ? (d.y < 0 ? -1 : 1) : 0
			};
		}
		t+=m;
	}
	static void run() {

		std::ifstream inputStream("2022/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		{
			char dir;
			int steps;
			while (inputStream >> dir >> steps)
			{
				dirs.emplace_back(dir, steps);
			}
		}
		inputStream.close();

		ivec2 head = {0,0};
		ivec2 tail = {0,0};
		std::unordered_set<ivec2, ivec2hash> visited;
		for (auto& [dir, steps] : dirs) {
			if (dir == 'R') {
				for (int i=0; i<steps; i++) {
					head.x++;
					follow(head, tail);
					visited.insert(tail);
				}
			}
			else if (dir == 'L') {
				for (int i=0; i<steps; i++) {
					head.x--;
					follow(head, tail);
					visited.insert(tail);
				}
			}
			else if (dir == 'D') {
				for (int i=0; i<steps; i++) {
					head.y++;
					follow(head, tail);
					visited.insert(tail);
				}
			}
			else if (dir == 'U') {
				for (int i=0; i<steps; i++) {
					head.y--;
					follow(head, tail);
					visited.insert(tail);
				}
			}
		}
		std::cout << visited.size() << std::endl;
	}
	static void runPart2() {
		std::vector<ivec2> segments(10, {0,0});
		std::unordered_set<ivec2, ivec2hash> visited;
		for (auto& [dir, steps] : dirs) {
			ivec2 d;
			switch (dir) {
				case 'R': d = ivec2(1, 0); break;
				case 'L': d = ivec2(-1, 0); break;
				case 'D': d = ivec2(0, 1); break;
				case 'U': d = ivec2(0, -1); break;
				default: d = ivec2(0, 0); break;
			}
			for (int i=0; i<steps; i++) {
				segments[0] += d;
				for (int j=1; j<segments.size(); j++) {
					follow(segments[j-1], segments[j]);
				}
				visited.insert(segments.back());
			}
		}
		std::cout << visited.size() << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/