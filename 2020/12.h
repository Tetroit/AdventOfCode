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

//ship controller with waypoint
struct Task
{
	static inline std::vector<std::pair<char, int>> instructions;
	static void run() {

		std::ifstream inputStream("2020/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		{
			char command;
			int val;
			while (inputStream >> command >> val)
			{
				if (command == 'R' || command == 'L')
					val/=90;
				instructions.emplace_back(command, val);
			}
			inputStream.close();
		}

		ivec2 pos = {0, 0};
		int dir = 0;
		for (auto& [command, val] : instructions) {
			switch (command) {
				case 'R': dir = (dir+3*val)%4; break;
				case 'L': dir = (dir+1*val)%4; break;
				case 'F': {
					auto d = Utils::UtilFacingVec[dir];
					pos += ivec2{d.first,d.second} * val;
					break;
				}
				case 'N': pos.y-=val; break;
				case 'S': pos.y+=val; break;
				case 'W': pos.x-=val; break;
				case 'E': pos.x+=val; break;

				default: break;
			}
		}
		std::cout << pos.len() << std::endl;
		//625 low
	}
	static void runPart2() {
		ivec2 pos = {0, 0};
		int dir = 0;
		ivec2 wp = {10, -1};
		for (auto& [command, val] : instructions) {
			switch (command) {
				case 'R': {
					for (int i=0; i<val%4; i++) {
						std::swap(wp.x, wp.y);
						wp.x = -wp.x;
					}
					break;
				}
				case 'L': {
					for (int i=0; i<val%4; i++) {
						std::swap(wp.x, wp.y);
						wp.y = -wp.y;
					}
					break;
				}
				case 'F':
					pos += wp * val;
					break;
				case 'N': wp.y-=val; break;
				case 'S': wp.y+=val; break;
				case 'W': wp.x-=val; break;
				case 'E': wp.x+=val; break;

				default: break;
			}
		}
		std::cout << pos.len() << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/