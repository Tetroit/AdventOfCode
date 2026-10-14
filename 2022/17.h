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
#include <vec.h>

#include "GridBase.h"

//tetris

struct Task
{
	struct Shape {
		std::vector<ivec2> cells;
		ivec2 dl;
		ivec2 ur;
		Shape(std::vector<ivec2> cells) {
			this->cells = cells;
			dl = ivec2{INT_MAX, INT_MAX};
			ur = ivec2{INT_MIN, INT_MIN};
			for (auto vec : cells) {
				if (vec.x < dl.x) dl.x = vec.x;
				if (vec.y < dl.y) dl.y = vec.y;
				if (vec.x > ur.x) ur.x = vec.x;
				if (vec.y > ur.y) ur.y = vec.y;
			}
		}

		bool canMoveRight(const DynamicGrid<uint8_t>& grid, ivec2 pos) {
			for (auto vec : cells) {
				ivec2 gPos = vec + pos;
				gPos.x++;
				if (!grid.inRange(gPos.x, gPos.y)) return false;
				if (grid.get(gPos.x, gPos.y)) return false;
			}
			return true;
		}
		bool canMoveDown(const DynamicGrid<uint8_t>& grid, ivec2 pos) {
			for (auto vec : cells) {
				ivec2 gPos = vec + pos;
				gPos.y--;
				if (!grid.inRange(gPos.x, gPos.y)) return false;
				if (grid.get(gPos.x, gPos.y)) return false;
			}
			return true;
		}
		bool canMoveLeft(const DynamicGrid<uint8_t>& grid, ivec2 pos) {
			for (auto vec : cells) {
				ivec2 gPos = vec + pos;
				gPos.x--;
				if (!grid.inRange(gPos.x, gPos.y)) return false;
				if (grid.get(gPos.x, gPos.y)) return false;
			}
			return true;
		}
		void freeze(DynamicGrid<uint8_t>& grid, ivec2 pos) {
			for (auto vec : cells) {
				ivec2 gPos = vec + pos;
				grid.set(gPos.x, gPos.y, true);
			}
		}
	};

	static inline std::string sequence;
	static inline std::vector<Shape> shapes;
	static void run() {

		std::ifstream inputStream("2022/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::getline(inputStream, sequence);
		inputStream.close();

		shapes = std::vector<Shape>{
			Shape({{0,0}, {1,0}, {2,0}, {3,0}}),
			Shape({{1,0}, {0,1}, {1,1}, {2,1}, {1,2}}),
			Shape({{0,0}, {1,0}, {2,0}, {2,1}, {2,2}}),
			Shape({{0,0}, {0,1}, {0,2}, {0,3}}),
			Shape({{0,0}, {1,0}, {0,1}, {1,1}}),
		};

		DynamicGrid<uint8_t> grid;
		grid.resize(7, 10);
		int nShape = 0;
		int nInstr = 0;
		int h = 0;
		bool spawnNew = true;
		ivec2 pos;
		while (nShape < 2022) {
			Shape& shape = shapes[nShape % 5];
			if (spawnNew) {
				pos = {2, h+3};
				spawnNew = false;
			}
			char dir = sequence[nInstr];
			nInstr = (nInstr + 1) % (int)sequence.size();
			if (dir == '<') {
				if (shape.canMoveLeft(grid, pos)) pos.x--;
			}
			else if (dir == '>') {
				if (shape.canMoveRight(grid, pos)) pos.x++;
			}
			if (shape.canMoveDown(grid, pos)) pos.y--;
			else {
				shape.freeze(grid, pos);
				nShape++;
				spawnNew = true;
				int shapeTop = shape.ur.y + pos.y + 1;
				if (shapeTop > h) h = shapeTop;
				if (grid.getHeight() < h + 10) grid.resize(7, h+10);
			}
		}
		std::cout << h << "\n";

	}

	static bool tryReduce(DynamicGrid<uint8_t>& grid, int& reduction) {
		std::vector<int> freeFall(grid.getWidth(), 0);
		for (int x=0; x<grid.getWidth(); x++) {
			for (int y=grid.getHeight()-1; y>=0; y--) {
				if (grid.get(x,y)) {
					freeFall[x] = y+1;
					break;
				}
			}
		}

		int max = freeFall[0];
		int min = freeFall[0];
		for (int i=1; i<freeFall.size(); i++) {
			const int& col = freeFall[i];
			if (freeFall[i] != min) return false;
			if (col < min) min = col;
			if (col > max) max = col;
		}
		if (min == 0) return false;
		int h = max - min;
		for (int y=0; y<h; y++) {
			for (int x=0; x<grid.getWidth(); x++) {
				grid.set(x,y, grid.get(x,y+min));
			}
		}
		grid.resize(grid.getWidth(), h);
		reduction = min;
		return true;
	}
	static inline std::vector<uint64_t> history;
	static uint64_t placeKey(int instructionID, int shape, int col, int relH) {
		uint64_t key = 0;
		key |= (uint64_t) instructionID;
		key <<= 3;
		key |= (uint64_t) shape;
		key <<= 3;
		key |= (uint64_t) col;
		key <<= 1;
		key |= (uint64_t) (relH < 0);
		key <<= 16;
		key |= (uint64_t) abs(relH);
		return key;
	}
	static int getDh(uint64_t key) {
		return ((key >> 16) & 1) == 1 ? - static_cast<int>(key & 0xFFFF) : static_cast<int>(key & 0xFFFF);
	}
	static void runPart2() {
		DynamicGrid<uint8_t> grid;
		grid.resize(7, 10);
		int nShape = 0;
		int nInstr = 0;
		int h = 0;
		int offset = 0;
		bool spawnNew = true;
		int previousY = 0;
		ivec2 pos;
		while (nShape < 10000) {
			Shape& shape = shapes[nShape % 5];
			if (spawnNew) {
				pos = {2, h+3};
				spawnNew = false;
			}
			char dir = sequence[nInstr];
			if (dir == '<') {
				if (shape.canMoveLeft(grid, pos)) pos.x--;
			}
			else if (dir == '>') {
				if (shape.canMoveRight(grid, pos)) pos.x++;
			}
			if (shape.canMoveDown(grid, pos)) pos.y--;
			else {
				shape.freeze(grid, pos);
				auto key = placeKey(nInstr, nShape%5, pos.x, pos.y - previousY);
				history.push_back(key);
				nShape++;
				spawnNew = true;
				int reduction = 0;
				// if (tryReduce(grid, reduction)) {
				// 	offset += reduction;
				// 	h = grid.getHeight();
				// 	grid.resize(grid.getWidth(), h+10);
				// }
				// else {
					int shapeTop = shape.ur.y + pos.y + 1;
					if (shapeTop > h) h = shapeTop;
					if (grid.getHeight() < h + 10) grid.resize(grid.getWidth(), h+10);
				// }

				// grid.print([](uint8_t val){return val == 0 ? '.' : '#';});
				previousY = pos.y;
			}
			nInstr = (nInstr + 1) % (int)sequence.size();
		}
		int matchSize = 10;
		int currentEt = 0;
		int repeatSize = 0;
		int repeatOffset = 0;
		std::cout << history.size() << "\n";
		bool loopFound = false;
		for (int hOff = 1; hOff < history.size() - matchSize; hOff++) {
			for (int i=hOff+matchSize; i < history.size(); i++) {
				if (history[i] == history[currentEt+hOff]) {
					if (currentEt == matchSize) {
						repeatOffset = hOff;
						repeatSize = i - hOff - currentEt;
						currentEt = 0;
						loopFound = true;

						std::cout << "repeat: " << repeatOffset << " + " << repeatSize << "n" << "\n";
						break;
					}
					currentEt++;
				}
				else {
					currentEt = 0;
				}
			}
			if (loopFound) break;
		}
		if (loopFound) {
			int offH = 0;
			int loopH = 0;
			for (int i=1; i<=repeatOffset; i++) {
				offH += getDh(history[i]);
			}
			for (int i=repeatOffset+1; i<=repeatOffset + repeatSize; i++) {
				loopH += getDh(history[i]);
			}
			std::cout << "repeat h: " << offH << " + " << loopH << "n" << "\n";

			static const uint64_t shapesN = 1000000000000;
			int restN = static_cast<int>((shapesN - repeatOffset) % repeatSize + repeatSize); // checking pre last loop cuz height of the rest might be less than last loop
			uint64_t nLoops = (shapesN - repeatOffset - restN)/repeatSize;
			int restH = 0;
			int hMax = 0;
			for (int i=repeatOffset+1; i <= repeatOffset + restN - 1; i++) {
				restH += getDh(history[i]);
				int top = shapes[i%5].ur.y + restH;
				if (top > hMax) hMax = top;
			}

			std::cout << nLoops * loopH + offH + hMax + 1 << "\n";
		}
		std::cout << h + offset << "\n";

		//1567630057799 high

	}
};

//-------------- NOTES AREA ----------------
/*

*/