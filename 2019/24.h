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

//Convay's game of life but on a tiny grid

struct Task
{
	using Region = Grid<5, 5, int>;

	static uint32_t getRating (const Region& grid) {
		uint32_t res = 0;
		for (int i=0; i<25; i++) {
			res |= grid.get(i%5, i/5) << i;
		}
		return res;
	}

	static int simulate(int val, int cnt) {
		if (val == 1 && cnt != 1) return 0;
		if (val == 0 && (cnt == 1 || cnt == 2)) return 1;
		return val;
	}
	static void run() {

		std::ifstream inputStream("2019/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		Region grid;
		Region grid2;
		std::string line;
		int readY = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(readY, line, [](char ch){return ch == '#' ? 1 : 0;});
			readY++;
		}
		inputStream.close();
		Region* src = &grid;
		Region* dst = &grid2;
		std::unordered_set<uint32_t> prevStates{getRating(grid)};
		while (true) {
			src->foreachCoord([&](int x, int y) {
				int cnt = src->countNeighbours4(x, y, [](int val){return val == 1;});
				dst->set(x, y, simulate(src->get(x, y), cnt));
			});
			auto rating = getRating(*dst);
			if (prevStates.contains(rating)) {
				std::cout << rating << std::endl;
				break;
			}
			prevStates.insert(rating);
			std::swap(src, dst);
		}
	}
	using RecurseGrid = std::unordered_map<int,Region>;
	static int countNeighbours(RecurseGrid& grid, int l, int x, int y) {

		int cnt = 0;
		//left
		if (x==0) cnt += grid[l-1].get(1, 2);
		else if (x==3 && y==2) {
			auto& inner = grid[l+1];
			for (int row = 0; row < 5; ++row)
				cnt += inner.get(4, row);
		}
		else cnt += grid[l].get(x-1, y);

		//right
		if (x==4) cnt += grid[l-1].get(3, 2);
		else if (x==1 && y==2) {
			auto& inner = grid[l+1];
			for (int row = 0; row < 5; ++row)
				cnt += inner.get(0, row);
		}
		else cnt += grid[l].get(x+1, y);

		//top
		if (y==0) cnt += grid[l-1].get(2, 1);
		else if (x==2 && y==3) {
			auto& inner = grid[l+1];
			for (int col = 0; col < 5; ++col)
				cnt += inner.get(col, 4);
		}
		else cnt += grid[l].get(x, y-1);

		//bottom
		if (y==4) cnt += grid[l-1].get(2, 3);
		else if (x==2 && y==1) {
			auto& inner = grid[l+1];
			for (int col = 0; col < 5; ++col)
				cnt += inner.get(col, 0);
		}
		else cnt += grid[l].get(x, y+1);

		return cnt;
	}
	static bool innerCircleTaken(Region& r) {
		if (r.get(1, 2) == 1) return true;
		if (r.get(3, 2) == 1) return true;
		if (r.get(2, 1) == 1) return true;
		if (r.get(2, 3) == 1) return true;
		return false;
	}
	static bool outerCircleTaken(Region& r) {
		for (int x=0; x<5; ++x) {
			if (r.get(x, 0) == 1) return true;
			if (r.get(x, 4) == 1) return true;
		}
		for (int y = 1; y<4; y++) {
			if (r.get(0, y) == 1) return true;
			if (r.get(4, y) == 1) return true;
		}
		return false;
	}
	static void runPart2() {
		std::ifstream inputStream("2019/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		RecurseGrid grid1;
		RecurseGrid grid2;
		std::string line;
		int readY = 0;
		while (std::getline(inputStream, line))
		{
			grid1[0].fillFromLine(readY, line, [](char ch){return ch == '#' ? 1 : 0;});
			readY++;
		}
		grid1[0].set(2,2,0);
		static constexpr int repeats = 200;
		RecurseGrid* src = &grid1;
		RecurseGrid* dst = &grid2;
		int top = 0;
		int bottom = 0;
		for (int i=0; i<repeats; i++) {
			if (innerCircleTaken(src->at(bottom))) {
				bottom++;
				src->emplace(bottom,Region());
			}
			if (outerCircleTaken(src->at(top))) {
				top--;
				src->emplace(top,Region());
			}
			for (auto&[l, region] : *src) {
				for (int x=0; x<5; x++) {
					for (int y=0; y<5; y++) {
						if (x==2 && y==2) continue;
						int cnt = countNeighbours(*src, l, x, y);
						dst->operator[](l).set(x, y, simulate(region.get(x, y), cnt));
					}
				}
			}
			std::swap(src, dst);
		}
		int cnt = 0;
		for (auto&[l, region] : *src) {
			cnt+=region.count([](int val){return val == 1;});
		}
		std::cout << cnt << std::endl;
		// for (int i=top; i<=bottom; i++) {
		// 	std::cout << i << std::endl;
		// 	src->at(i).print([](int x){return x==1 ? '#' : '.';});
		// }
	}
};

//-------------- NOTES AREA ----------------
/*

*/