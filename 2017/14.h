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

//hash grid

struct Task
{
	template <typename T, int arr_s>
	static void reverse(std::array<T, arr_s>& vec, int start, int size) {
		int end = (start + size - 1) % arr_s;
		int l = start;
		int r = end;
		for (int i = 0; i < size; i+=2) {
			std::swap(vec[r], vec[l]);
			r--;
			if (r < 0)
				r+= arr_s;
			l++;
			if (l >= vec.size())
				l -= arr_s;
		}
	}
	static std::array<uint8_t, 16> knotHash(std::string key) {

		key+=std::string{17, 31, 73, 47, 23};
		std::array<uint8_t, 256> ring{};
		for (int i=0; i<ring.size(); i++) {
			ring[i] = i;
		}
		int currentPos = 0;
		int currentStride = 0;
		for (int i = 0; i<64; i++) {
			for (uint8_t loop : key) {
				reverse<uint8_t, 256>(ring, currentPos, loop);
				currentPos = (currentPos + loop + currentStride) & 255;
				currentStride++;
			}
		}
		std::array<uint8_t, 16> dense{};
		for (int i=0; i<16; i++) {
			uint8_t hash = 0;
			for (int j=0; j<16; j++) {
				hash ^= ring[i*16+j];
			}
			dense[i] = hash;
		}
		return dense;
	}

	static inline Grid<128, 128, bool> disk;
	static void run() {
		disk.clear(false);
		std::string line = "stpzcrnm";
		for (int i=0; i<128; i++) {
			std::array<uint8_t, 16> hash = knotHash(line + "-" + std::to_string(i));
			for (int j=0; j<128; j++) {
				const int& cell = hash[j/8];
				bool val = (cell >> (7 - (j&7))) & 1;
				disk.set(j, i, val);
			}
		}
		std::cout << disk.count([](bool x){return x;}) << std::endl;
	}
	static void runPart2() {
		Grid<128, 128, bool> visited;
		visited.clear(false);
		int cnt = 0;
		disk.foreachCoord([&](int x, int y) {
			if (!disk.get(x,y)) return;
			if (visited.get(x,y)) return;
			visited.set(x,y,true);
			cnt++;
			std::deque<std::pair<int, int>> queue {{x,y}};
			while (!queue.empty()) {
				auto[currX, currY] = queue.front();
				queue.pop_front();
				for (auto& [nx, ny] : disk.getNeighbourCoords4(currX,currY)) {
					if (visited.get(nx,ny)) continue;
					if (disk.get(nx,ny)) queue.emplace_back(nx,ny);
					visited.set(nx,ny,true);
				}
			}
		});
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/