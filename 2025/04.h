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

//The task about picking up paper
struct Task
{
	static bool isPaper(const std::vector<std::string>& grid, int width, int height, int x, int y) {
		if (!Utils::inRange(width, height, x, y)) return false;
		return grid[y][x] == '@';
	}
	static int countNeighbours(const std::vector<std::string>& grid, int width, int height, int x, int y) {
		int cnt = 0;
		if (isPaper(grid, width, height, x-1, y-1)) cnt++;
		if (isPaper(grid, width, height, x, y-1)) cnt++;
		if (isPaper(grid, width, height, x+1, y-1)) cnt++;
		if (isPaper(grid, width, height, x-1, y)) cnt++;
		if (isPaper(grid, width, height, x+1, y)) cnt++;
		if (isPaper(grid, width, height, x-1, y+1)) cnt++;
		if (isPaper(grid, width, height, x, y+1)) cnt++;
		if (isPaper(grid, width, height, x+1, y+1)) cnt++;
		return cnt;
	}
	static int countLiftable(const std::vector<std::string>& grid, int width, int height) {
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (isPaper(grid, width, height, j, i) && countNeighbours(grid, width, height, j, i) < 4) {
					cnt++;
				}
			}
		}
		return cnt;
	}
	static int pickUpPaper(std::vector<std::string>& grid, int width, int height) {
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (isPaper(grid, width, height, j, i) && countNeighbours(grid, width, height, j, i) < 4) {
					cnt++;
					grid[i][j] = 'x';
				}
			}
		}
		return cnt;
	}
	static void run() {

		std::ifstream inputStream("2025/input/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::string> grid;
		std::string line;
		while (std::getline(inputStream, line))
		{
			grid.push_back(line);
		}
		int height = grid.size();
		int width = grid[0].size();
		int liftableCount = countLiftable(grid, width, height);
		std::cout << liftableCount << std::endl;
		inputStream.close();
	}
	static void runPart2() {
		std::ifstream inputStream("2025/input/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::string> grid;
		std::string line;
		while (std::getline(inputStream, line))
		{
			grid.push_back(line);
		}
		int height = grid.size();
		int width = grid[0].size();
		int iterations = 0;
		int totalCount = 0;
		int liftedCount = pickUpPaper(grid, width, height);
		while (liftedCount > 0) {
			totalCount += liftedCount;
			iterations++;
			liftedCount = pickUpPaper(grid, width, height);
		}
		std::cout << totalCount << std::endl;
		inputStream.close();
	}
};

//-------------- NOTES AREA ----------------
/*

*/