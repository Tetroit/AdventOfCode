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

//Sliding rocks and support beams

struct GridKey {
	int w, h;
	std::vector<uint64_t> data;

	friend bool operator==(const GridKey &lhs, const GridKey &rhs) {
		if (lhs.w != rhs.w || lhs.h != rhs.h) return false;
		return lhs.data == rhs.data;
	}

	friend bool operator!=(const GridKey &lhs, const GridKey &rhs) {
		return !(lhs == rhs);
	}
};
struct Task
{
	static inline DynamicGrid<char> initMap;
	static void run() {

		std::ifstream inputStream("2023/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		initMap.fillFromStream(inputStream, DefaultConvert<char>());
		inputStream.close();

		int load = 0;
		for (int x = 0; x<initMap.getWidth(); x++) {
			int stack = 0;
			for (int y = initMap.getHeight() - 1; y >= 0; y--) {
				switch (initMap.get(x,y)) {
					case 'O': {
						load += initMap.getHeight() - y;
						stack++;
						break;
					}
					case '.': {
						load+=stack;
						break;
					}
					case '#': {
						stack = 0;
						break;
					}
				}
			}
		}
		std::cout << load << std::endl;
	}
	static GridKey getCycleKey(const DynamicGrid<char>& grid) {
		GridKey key;
		key.data.resize((grid.getWidth() * grid.getHeight() + 63) / 64 );
		key.w = grid.getWidth();
		key.h = grid.getHeight();
		for (int x = 0; x < grid.getWidth(); x++) {
			for (int y = 0; y < grid.getHeight(); y++) {
				if (grid.get(x,y) != 'O') continue;
				int bitID = x + y * grid.getWidth();
				int chunk = bitID / 64;
				int local = bitID % 64;
				key.data[chunk] |= 1ULL << local;
			}
		}
		return key;
	}
	static int decodeKeyLoad(const GridKey& key) {
		int load = 0;
		for (int x = 0; x < key.w; x++) {
			for (int y = 0; y < key.h; y++) {
				int bitID = x + y * key.w;
				int chunk = bitID / 64;
				int local = bitID % 64;
				bool isStone = ((key.data[chunk] & 1ULL << local) != 0);
				if (isStone) load += key.h - y;
			}
		}
		return load;
	}
	static void runPart2() {
		DynamicGrid<char> grid1 = initMap;
		DynamicGrid<char> grid2 = initMap;
		DynamicGrid<char>* src = &grid1;
		DynamicGrid<char>* dst = &grid2;
		int dir = 0;
		std::vector<GridKey> seq;
		while (true) {
			dst->clear('.');
			if (dir == 0) {
				for (int x = 0; x<initMap.getWidth(); x++) {
					int stack = 0;
					for (int y = initMap.getHeight() - 1; y >= 0; y--) {
						char cell = src->get(x,y);
						if (cell == 'O')
							stack++;
						else if (cell == '#'){
							dst->set(x,y,'#');
							for (int i = 1; i<=stack; i++) {
								dst->set(x,y+i,'O');
							}
							stack = 0;
						}
					}
					for (int i = 0; i<stack; i++) {
						dst->set(x,i,'O');
					}
				}
			}
			else if (dir == 1) {
				for (int y = 0; y<initMap.getHeight(); y++) {
					int stack = 0;
					for (int x = initMap.getWidth() - 1; x >= 0; x--) {
						char cell = src->get(x,y);
						if (cell == 'O')
							stack++;
						else if (cell == '#'){
							dst->set(x,y,'#');
							for (int i = 1; i<=stack; i++) {
								dst->set(x+i,y,'O');
							}
							stack = 0;
						}
					}
					for (int i = 0; i<stack; i++) {
						dst->set(i,y,'O');
					}
				}
			}
			else if (dir == 2) {
				for (int x = 0; x<initMap.getWidth(); x++) {
					int stack = 0;
					for (int y = 0; y < initMap.getHeight(); y++) {
						char cell = src->get(x,y);
						if (cell == 'O')
							stack++;
						else if (cell == '#'){
							dst->set(x,y,'#');
							for (int i = 1; i<=stack; i++) {
								dst->set(x,y-i,'O');
							}
							stack = 0;
						}
					}
					for (int i = 1; i<=stack; i++) {
						dst->set(x,initMap.getHeight()-i,'O');
					}
				}
			}
			else if (dir == 3) {
				for (int y = 0; y<initMap.getHeight(); y++) {
					int stack = 0;
					for (int x = 0; x < initMap.getWidth(); x++) {
						char cell = src->get(x,y);
						if (cell == 'O')
							stack++;
						else if (cell == '#'){
							dst->set(x,y,'#');
							for (int i = 1; i<=stack; i++) {
								dst->set(x-i,y,'O');
							}
							stack = 0;
						}
					}
					for (int i = 1; i<=stack; i++) {
						dst->set(initMap.getWidth()-i,y,'O');
					}
				}
			}
			dir = (dir + 1) % 4;

			std::swap(src,dst);


			if (dir == 0) {
				GridKey key = getCycleKey(*src);
				bool found = false;
				int offset = 0;
				for (int i=0; i<seq.size(); i++) {
					auto& entry = seq[i];
					if (entry == key) {
						found = true;
						offset = i;
						break;
					}
				}
				if (found) {
					int loop = seq.size() - offset;
					long long cycles = 1000000000;
					int res = (cycles - 1 - offset) % loop + offset;
					std::cout << decodeKeyLoad(seq[res]) << std::endl;
					break;
				}
				seq.push_back(key);
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/