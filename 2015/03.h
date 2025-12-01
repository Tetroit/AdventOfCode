#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

struct Task
{

	void run() {


		std::ifstream inputStream("03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		int x = 0;
		int y = 0;
		int roboX = 0;
		int roboY = 0;

		std::vector<int> map {0};

		char dir;
		while (inputStream >> dir)
		{
			if (dir == '>')
				x++;
			else if (dir == '<')
				x--;
			else if (dir == '^')
				y++;
			else if (dir == 'v')
				y--;

			inputStream >> dir;

			int loc = (x << 8) + y;
			if (std::find(map.begin(), map.end(), loc) ==  map.end()) {
				map.push_back(loc);
			}

			if (dir == '>')
				roboX++;
			else if (dir == '<')
				roboX--;
			else if (dir == '^')
				roboY++;
			else if (dir == 'v')
				roboY--;


			loc = (roboX << 8) + roboY;
			if (std::find(map.begin(), map.end(), loc) ==  map.end()) {
				map.push_back(loc);
			}
		}
		std::cout << map.size() << "\n";
	}
};
