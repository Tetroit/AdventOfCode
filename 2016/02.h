#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "grid.h"
#include "utils.h"

//walking on code pad
struct Task
{
	static inline Grid<3,3,int> grid {
		1,2,3,
		4,5,6,
		7,8,9
	};
	static inline Grid<5,5,char> grid2{
		'0','0','1','0','0',
		'0','2','3','4','0',
		'5','6','7','8','9',
		'0','A','B','C','0',
		'0','0','D','0','0'
	};
	static void run() {

		std::ifstream inputStream("2016/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string code = "";
		int x = 1;
		int y = 1;
		while (std::getline(inputStream, line))
		{
			for (const char& ch : line) {
				if (ch == 'U') {
					y--;
				}
				if (ch == 'D') {
					y++;
				}
				if (ch == 'L') {
					x--;
				}
				if (ch == 'R') {
					x++;
				}
				std::tie(x,y) = grid.clamp(x,y);
			}
			code +=	std::to_string(grid.get(x,y));
		}
		inputStream.close();
		std::cout << code << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2016/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::string code = "";
		int x = 0;
		int y = 2;
		while (std::getline(inputStream, line))
		{
			for (const char& ch : line) {
				if (ch == 'U') {
					if (grid2.inRange(x,y-1) && grid2.get(x,y-1) != '0') {
						y--;
					}
				}
				if (ch == 'D') {
					if (grid2.inRange(x,y+1) && grid2.get(x,y+1) != '0') {
						y++;
					}
				}
				if (ch == 'L') {
					if (grid2.inRange(x-1,y) && grid2.get(x-1,y) != '0') {
						x--;
					}
				}
				if (ch == 'R') {
					if (grid2.inRange(x+1,y) && grid2.get(x+1,y) != '0') {
						x++;
					}
				}
			}
			code +=	grid2.get(x,y);
		}
		inputStream.close();
		std::cout << code << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/