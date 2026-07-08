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

#include "GridBase.h"
#include "utils.h"

//TODO: put a small description of the task
struct Task
{
	static constexpr int input = 368078;
	static void run() {
		int sum = 1;
		int circle = 8;
		int rad = 1;
		int nextSum = sum + circle;
		while (input > nextSum) {
			circle += 8;
			sum = nextSum;
			nextSum = sum + circle;
			rad ++;
		}
		int offset = input - sum;
		int side = circle >> 2;
		offset %= side;
		offset = abs(offset - (side / 2));
		std::cout << rad + offset << std::endl;
	}
	static void runPart2() {
		Grid<99, 99, int> grid;
		grid.clear(0);
		grid.set(49, 49, 1);
		int posX = 49;
		int posY = 49;
		int dir = 0;
		int nextDir = 1;
		auto [dirX, dirY] = Utils::UtilFacingVec[dir];
		auto [leftX, leftY] = Utils::UtilFacingVec[nextDir];

		while (grid.get(posX, posY) < input) {
			posX += dirX;
			posY += dirY;
			int& cell = grid.getRef(posX, posY);
			for (auto [x, y] : grid.getNeighbourCoords8(posX, posY)) {
				cell+=grid.get(x, y);
			}
			if (cell > input) {
				std::cout << cell << std::endl;
				break;
			}
			int checkX = posX + leftX;
			int checkY = posY + leftY;
			if (grid.get(checkX, checkY) == 0) {
				dir = nextDir;
				nextDir = (nextDir + 1)%4;
				std::tie(dirX, dirY) = Utils::UtilFacingVec[dir];
				std::tie(leftX, leftY) = Utils::UtilFacingVec[nextDir];
			}
		}

	}
};

//-------------- NOTES AREA ----------------
/*

*/