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
#include "vec.h"

//3d(4d) cellular automata

struct Grid3D {
	std::unordered_map<ivec3, uint8_t, ivec3hash> data;

	int countNeighbours(ivec3 pos) const {
		int cnt = 0;
		for (int i=pos.x-1; i<=pos.x+1; i++) {
			for (int j=pos.y-1; j<=pos.y+1; j++) {
				for (int k=pos.z-1; k<=pos.z+1; k++) {
					auto ptr = ivec3{i,j,k};
					if (ptr == pos) continue;
					if (!data.contains(ptr)) continue;
					if (data.at(ptr) == 1) cnt++;
				}
			}
		}
		return cnt;
	}
	void expandPing() {
		for (auto [pos, val] : data) {
			if (val == 1) {
				for (int i=pos.x-1; i<=pos.x+1; i++) {
					for (int j=pos.y-1; j<=pos.y+1; j++) {
						for (int k=pos.z-1; k<=pos.z+1; k++) {
							data[{i,j,k}];
						}
					}
				}
			}
		}
	}
	void print() {
		ivec3 min = {INT_MAX, INT_MAX, INT_MAX};
		ivec3 max = {INT_MIN, INT_MIN, INT_MIN};
		for (auto[pos, val] : data) {
			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;
			if (pos.z > max.z) max.z = pos.z;
			if (pos.x < min.x) min.x = pos.x;
			if (pos.y < min.y) min.y = pos.y;
			if (pos.z < min.z) min.z = pos.z;
		}
		for (int z=min.z; z<=max.z; z++) {
			std::cout << "z=" << z << '\n';
			for (int y=min.y; y<=max.y; y++) {
				for (int x=min.x; x<=max.x; x++) {
					if (data.contains({x,y,z})) {
						std::cout << (data[{x,y,z}] == 1 ? '#' : '.');
					}
					else {
						std::cout << '.';
					}
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
		std::cout << std::endl;
	}
};

struct Grid4D {
	std::unordered_map<ivec4, uint8_t, ivec4hash> data;

	int countNeighbours(ivec4 pos) const {
		int cnt = 0;
		for (int i=pos.x-1; i<=pos.x+1; i++) {
			for (int j=pos.y-1; j<=pos.y+1; j++) {
				for (int k=pos.z-1; k<=pos.z+1; k++) {
					for (int l=pos.w-1; l<=pos.w+1; l++) {
						auto ptr = ivec4{i,j,k,l};
						if (ptr == pos) continue;
						if (!data.contains(ptr)) continue;
						if (data.at(ptr) == 1) cnt++;
					}
				}
			}
		}
		return cnt;
	}
	void expandPing() {
		for (auto [pos, val] : data) {
			if (val == 1) {
				for (int i=pos.x-1; i<=pos.x+1; i++) {
					for (int j=pos.y-1; j<=pos.y+1; j++) {
						for (int k=pos.z-1; k<=pos.z+1; k++) {
							for (int l=pos.w-1; l<=pos.w+1; l++) {
								data[{i,j,k,l}];
							};
						}
					}
				}
			}
		}
	}
};

struct Task
{
	static void run() {

		std::ifstream inputStream("2020/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		Grid3D grid = Grid3D();
		Grid3D grid2 = Grid3D();

		Grid3D* src = &grid;
		Grid3D* dst = &grid2;
		std::string line;
		int ry = 0;
		while (std::getline(inputStream, line))
		{
			for (int rx=0; rx<line.size(); rx++) {
				if (line[rx] == '#')
					grid.data[{rx,ry,0}] = 1;
			}
			ry++;
		}
		inputStream.close();
		for (int i=0; i<6; i++) {
			dst->data.clear();
			src->expandPing();
			for (const auto& [pos, val] : src->data) {
				int neighbours = src->countNeighbours(pos);
				if ((val == 1 && (neighbours == 2 || neighbours == 3)) || neighbours == 3) {
					dst->data[pos] = 1;
				}
			}
			std::swap(src, dst);
		}
		int cnt = 0;
		for (const auto& [pos, val] : src->data) {
			cnt += val;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2020/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		Grid4D grid = Grid4D();
		Grid4D grid2 = Grid4D();

		Grid4D* src = &grid;
		Grid4D* dst = &grid2;
		std::string line;
		int ry = 0;
		while (std::getline(inputStream, line))
		{
			for (int rx=0; rx<line.size(); rx++) {
				if (line[rx] == '#')
					grid.data[{rx,ry,0,0}] = 1;
			}
			ry++;
		}
		inputStream.close();
		for (int i=0; i<6; i++) {
			dst->data.clear();
			src->expandPing();
			for (const auto& [pos, val] : src->data) {
				int neighbours = src->countNeighbours(pos);
				if ((val == 1 && (neighbours == 2 || neighbours == 3)) || neighbours == 3) {
					dst->data[pos] = 1;
				}
			}
			std::swap(src, dst);
		}
		int cnt = 0;
		for (const auto& [pos, val] : src->data) {
			cnt += val;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	my approach is not the most efficient because of the hashes and memory reallocation
	but probably the most cache friendly and easy to extend
*/