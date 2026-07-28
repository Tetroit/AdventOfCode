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

//drones forming a message

struct Drone {
	int x, y;
	int vx, vy;
};
struct Task
{
	static inline std::vector<Drone> drones;
	static void run() {

		std::ifstream inputStream("2018/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::istringstream lineStream(line);
			Drone drone;
			lineStream.ignore(sizeof("position=<") - 1);
			lineStream >> drone.x;
			lineStream.ignore(1);
			lineStream >> drone.y;
			lineStream.ignore(sizeof("> velocity=<")-1);
			lineStream >> drone.vx;
			lineStream.ignore(1);
			lineStream >> drone.vy;
			drones.push_back(drone);
		}
		inputStream.close();
		std::string comm;
		int sec = 0;
		do {
			std::cin >> comm;
			int arg;
			if (comm == "e") break;
			else if (comm == "+") {
				std::cin >> arg;
				for (auto& drone : drones) {
					drone.x += arg * drone.vx;
					drone.y += arg * drone.vy;
				}
				sec += arg;
			}
			else if (comm == "-") {
				std::cin >> arg;
				for (auto& drone : drones) {
					drone.x -= arg * drone.vx;
					drone.y -= arg * drone.vy;
				}
				sec -= arg;
			}
			else if (comm == "bb") {
				int minX = std::numeric_limits<int>::max();
				int minY = std::numeric_limits<int>::max();
				int maxX = std::numeric_limits<int>::min();
				int maxY = std::numeric_limits<int>::min();
				for (auto& drone : drones) {
					minX = std::min(drone.x, minX);
					minY = std::min(drone.y, minY);
					maxX = std::max(drone.x, maxX);
					maxY = std::max(drone.y, maxY);
				}
				int sizeX = maxX - minX + 1;
				int sizeY = maxY - minY + 1;
				std::cout <<
					"Min: <" << minX << ", " << minY <<
					">\nMax: <" << maxX << ", " << maxY <<
					">\nSize: <" << sizeX << ", " << sizeY << ">" << std::endl;
			}
			else if (comm == "p") {
				int minX = std::numeric_limits<int>::max();
				int minY = std::numeric_limits<int>::max();
				int maxX = std::numeric_limits<int>::min();
				int maxY = std::numeric_limits<int>::min();
				for (auto& drone : drones) {
					minX = std::min(drone.x, minX);
					minY = std::min(drone.y, minY);
					maxX = std::max(drone.x, maxX);
					maxY = std::max(drone.y, maxY);
				}
				int sizeX = maxX - minX + 1;
				int sizeY = maxY - minY + 1;
				if (sizeX > 1000 || sizeY > 1000) {
					std::cout << "Display too large" << std::endl;
				}
				else {
					DynamicGrid<uint8_t> grid;
					grid.resize(sizeX, sizeY);
					grid.clear(0);
					for (auto& drone : drones) {
						grid.set(drone.x - minX, drone.y - minY, 1);
					}
					grid.print([](uint8_t val){return val == 1 ? '#' : '.';});
				}
			}
			else if (comm == "s") {
				std::cout << sec << std::endl;
			}
		} while (true);
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

THIS IS A CONSOLE APP so no part 2

+ moves N seconds forward into simulation
- moves N seconds back
bb - bounding box
p - print
s - display current simulation second
e - exit

*/