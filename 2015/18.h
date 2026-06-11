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
#include "grid.h"

//Conway's game of life with lights

//I got tired of writing grid struct every time so now there is very sophisticated grid class lol
struct Task
{
	static constexpr int gridSize = 100;
	static constexpr int steps = 100;
	using LightGrid = Grid <gridSize,gridSize,uint8_t>;
	static inline LightGrid grid;

	static void step(LightGrid& _grid) {
		LightGrid newGrid;
		newGrid.clear(0);
		_grid.foreachCoord([&](int x, int y) {
			int n = _grid.countNeighbours8(x, y, [](uint8_t val) {return val == 1;});
			if (_grid.get(x,y) == 1 && (n == 2 || n == 3) || _grid.get(x,y) == 0 && n == 3) {
				newGrid.set(x,y,1);
			}
		});
		_grid.copyFrom(newGrid);
		// _grid.print([](uint8_t val){return val == 1 ? '#' : '.';});
		// std::cout << std::endl;
	}
	static void step_corners(LightGrid& _grid) {
		LightGrid newGrid;
		newGrid.clear(0);
		_grid.foreachCoord([&](int x, int y) {
			//corner condition
			if ((x == 0 || x == gridSize-1) && ( y == 0 || y == gridSize-1)) {
				newGrid.set(x,y,1);
				return;
			}
			//neighbour condition
			int n = _grid.countNeighbours8(x, y, [](uint8_t val) {return val == 1;});
			if (_grid.get(x,y) == 1 && (n == 2 || n == 3) || _grid.get(x,y) == 0 && n == 3) {
				newGrid.set(x,y,1);
			}
		});
		_grid.copyFrom(newGrid);
		// _grid.print([](uint8_t val){return val == 1 ? '#' : '.';});
		// std::cout << std::endl;
	}
	static void run() {

		std::ifstream inputStream("2015/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int nLine = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(nLine, line, [](char ch){return ch == '#' ? 1 : 0;} );
			nLine++;
		}
		inputStream.close();
		for (int i=0; i<steps; i++) {
			step(grid);
		}
		std::cout << grid.count([](uint8_t val){return val == 1;}) << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2015/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int nLine = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(nLine, line, [](char ch){return ch == '#' ? 1 : 0;} );
			nLine++;
		}
		inputStream.close();
		for (int i=0; i<steps; i++) {
			step_corners(grid);
		}
		std::cout << grid.count([](uint8_t val){return val == 1;}) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/