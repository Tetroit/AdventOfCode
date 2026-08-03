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

//cellular automata
struct Task
{
	static void simulate(int x, int y, DynamicGrid<char>& src, DynamicGrid<char>& dst) {
		switch (src.get(x,y)) {
			case '.': {
				if (src.countNeighbours8(x,y,[](char ch){return ch == '|';})>=3)
					dst.set(x,y,'|');
				else
					dst.set(x,y,'.');
				break;
			}
			case '|': {
				if (src.countNeighbours8(x,y,[](char ch){return ch == '#';})>=3)
					dst.set(x,y,'#');
				else
					dst.set(x,y,'|');
				break;
			}
			case '#': {
				if (src.countNeighbours8(x,y,[](char ch){return ch == '#';})==0 ||
					src.countNeighbours8(x,y,[](char ch){return ch == '|';})==0)
					dst.set(x,y,'.');
				else
					dst.set(x,y,'#');
				break;
			}
		}
	};
	static void run() {

		std::ifstream inputStream("2018/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		DynamicGrid<char> grid;
		std::string line;
		int lineN = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(lineN, line, [](char ch){return ch;});
			lineN++;
		}
		inputStream.close();
		DynamicGrid<char> grid2 = grid;

		for (int i=0; i<10; i++) {
			DynamicGrid<char>& src = i&1 ? grid2 : grid;
			DynamicGrid<char>& dst = i&1 ? grid : grid2;
			src.foreachCoord([&](int x, int y){simulate(x,y,src,dst);});
		}
		std::cout << grid.count([](char ch){return ch=='|';}) * grid.count([](char ch){return ch=='#';}) << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2018/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		DynamicGrid<char> grid;
		std::string line;
		int lineN = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(lineN, line, [](char ch){return ch;});
			lineN++;
		}
		inputStream.close();
		DynamicGrid<char> grid2 = grid;
		std::vector<DynamicGrid<char>> grids {grid};
		auto findIteration = [&](const DynamicGrid<char>& src) {
			for (int i=0; i<grids.size(); ++i) {
				bool match = true;
				for (int x=0; x<src.getWidth(); x++) {
					for (int y=0; y<src.getHeight(); y++) {
						if (grids[i].get(x,y)!=src.get(x,y)) {
							match = false;
							break;
						}
					}
					if (!match) break;
				}
				if (match) return i;
			}
			return -1;
		};
		int offset = -1;
		int loop = -1;
		for (int i=0;; i++) {
			DynamicGrid<char>& src = i&1 ? grid2 : grid;
			DynamicGrid<char>& dst = i&1 ? grid : grid2;
			src.foreachCoord([&](int x, int y){simulate(x,y,src,dst);});
			std::cout << dst.count([](char ch){return ch=='|';}) << " " << dst.count([](char ch){return ch=='#';}) << std::endl;
			int repeat = findIteration(dst);
			if (repeat != -1) {
				offset = repeat;
				loop = i - repeat + 1;
				break;
			}
			grids.emplace_back(dst);
		}
		int n = 1000000000;
		std::cout << offset << " " << loop << std::endl;
		auto& res = grids[(n-offset) % loop + offset];
		res.print([](char ch){return ch;});
		std::cout << res.count([](char ch){return ch=='|';}) * res.count([](char ch){return ch=='#';}) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/