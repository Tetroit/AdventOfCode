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

//finding largest 3x3 sum

struct Task
{
	static constexpr int input = 5093;
	static inline Grid<300,300,int> grid;
	static void run() {
		grid.foreachCoord([](int x, int y) {
			int rackID = x + 11;
			grid.set(x,y,(rackID * (y + 1) + input) * rackID / 100 % 10 - 5);
		});
		int max = -100;
		int maxX = -1;
		int maxY = -1;
		for (int x = 0; x<grid.getWidth() - 2; x++) {
			for (int y = 0; y<grid.getHeight() - 2; y++) {
				int sum = 0;
				for (int dx = 0; dx<3; dx++) {
					for (int dy = 0; dy<3; dy++) {
						sum += grid.get(x+dx, y+dy);
					}
				}
				if (sum > max) {
					max = sum;
					maxX = x+1;
					maxY = y+1;
				};
			}
		}
		std::cout << maxX << ',' << maxY << std::endl;
	}
	static void findLargestForCol(const Grid<300,300,int>& grid, std::tuple<int,int,int>& res, int x) {
		int& maxSum = std::get<0>(res);
		int& maxY = std::get<1>(res);
		int& maxSize = std::get<2>(res);
		for (int y = 0; y<grid.getHeight(); y++) {
			int maxAvail = std::min(grid.getWidth() - x, grid.getHeight() - y);
			int sum = 0;
			for (int size = 1; size <= maxAvail; size++) {
				for (int dx = 0; dx<size-1; dx++) {
					sum += grid.get(x+dx, y+size-1);
				}
				for (int dy = 0; dy<size; dy++) {
					sum += grid.get(x+size-1, y+dy);
				}
				if (sum > maxSum) {
					maxY = y+1;
					maxSum = sum;
					maxSize = size;
				}
			}
		}
	}
	static void runPart2() {
		std::vector<std::tuple<int,int,int>> largest(300, {std::numeric_limits<int>::min(), -1, -1});
		std::vector<std::thread> jobs;
		for (int x = 0; x<grid.getWidth(); x++) {
			jobs.emplace_back(findLargestForCol, std::cref(grid), std::ref(largest[x]), x);
		}
		for (auto& job : jobs) {
			job.join();
		}
		int maxSum = std::numeric_limits<int>::min();
		int maxX = -1;
		int maxY = -1;
		int maxSize = -1;
		for (int x=0; x<largest.size(); x++) {
			const auto& [sum, y, size] = largest[x];
			if (sum > maxSum) {
				maxX = x;
				maxY = y;
				maxSize = size;
				maxSum = sum;
			}
		}
		std::cout << maxX << ',' << maxY << ',' << maxSize << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	having fun with multithreading
*/