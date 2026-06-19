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
#include "utils.h"

//jumping on the grid in directions
struct Task
{
	//bit grid would perform faster for part 2 but me = too lazy
	static inline std::vector<std::pair<int, int>> coords;
	static void run() {

		std::ifstream inputStream("2016/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int x = 0, y = 0;
		char dir;
		char shit;
		int facing = 3;
		while (inputStream >> dir)
		{
			if (dir == 0)
				break;
			int amount;
			inputStream >> amount >> shit;
			if (dir == 'R')
				facing = (facing + 1) % 4;
			if (dir == 'L')
				facing = (facing + 3) % 4;

			if (facing == 0) {
				x+=amount;
			}
			else if (facing == 1) {
				y-=amount;
			}
			else if (facing == 2) {
				x-=amount;
			}
			else if (facing == 3) {
				y+=amount;
			}
		}
		std::cout << abs(x) + abs(y) << std::endl;
		inputStream.close();
	}
	static void runPart2() {
		std::ifstream inputStream("2016/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int x = 0, y = 0;
		char dir;
		char shit;
		int facing = 3;
		while (inputStream >> dir)
		{
			auto check = [](int x_, int y_) -> bool{
				if (std::ranges::find(coords, std::pair{x_,y_}) != coords.end()) {
					std::cout << abs(x_) + abs(y_) << std::endl;
					return true;
				}
				return false;
			};
			if (dir == 0)
				break;
			int amount;
			inputStream >> amount >> shit;
			if (dir == 'R')
				facing = (facing + 1) % 4;
			if (dir == 'L')
				facing = (facing + 3) % 4;

			if (facing == 0) {
				for (int i = x; i<x+amount; i++) {
					if (check(i,y))
						return;
					coords.emplace_back(i, y);
				}
				x+=amount;
			}
			else if (facing == 1) {
				for (int i = y; i>y-amount; i--) {
					if (check(x,i))
						return;
					coords.emplace_back(x, i);
				}
				y-=amount;
			}
			else if (facing == 2) {
				for (int i = x; i>x+amount; i--) {
					if (check(i,y))
						return;
					coords.emplace_back(i, y);
				}
				x-=amount;
			}
			else if (facing == 3) {
				for (int i = y; i<y+amount; i++) {
					if (check(x,i))
						return;
					coords.emplace_back(x, i);
				}
				y+=amount;
			}

		}
		inputStream.close();
	}
};

//-------------- NOTES AREA ----------------
/*

*/