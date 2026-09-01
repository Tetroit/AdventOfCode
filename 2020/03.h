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

//counting trees in dx=3 dy=1

struct Task
{
	static inline DynamicGrid<int> grid;
	static void run() {

		std::ifstream inputStream("2020/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int ry=0;
		std::string line;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(ry, line, [](char ch){return ch == '#' ? 1 : 0;});
			ry++;
		}
		inputStream.close();
		int x = 0;
		int cnt = 0;
		for (int y =0; y < grid.getHeight(); y++) {
			cnt += grid.get(x,y);
			x = (x + 3) % grid.getWidth();
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		const std::vector<std::pair<int,int>> slopes{
			{1,1},
			{3,1},
			{5,1},
			{7,1},
			{1,2}
		};

		long long mult = 1;
		for (auto& [dx,dy] : slopes) {
			int cnt = 0;
			int x = 0;
			for (int y =0; y < grid.getHeight(); y+=dy) {
				cnt += grid.get(x,y);
				x = (x + dx) % grid.getWidth();
			}
			mult *= cnt;
		}
		std::cout << mult << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/