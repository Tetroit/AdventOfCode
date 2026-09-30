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

//enabling disabling cubes

struct Task
{
	struct Cuboid {
		ivec3 min;
		ivec3 max;
		[[nodiscard]] long long size() const{
			ivec3 d = max - min + ivec3{1,1,1};
			return static_cast<long long>(d.x) * d.y * d.z;
		}
	};
	struct Instruction {
		Cuboid bounds;
		bool state{};
	};
	static inline std::vector<Instruction> instructions;
	static void run() {

		std::ifstream inputStream("2021/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			Instruction instruction;
			std::string word;
			std::istringstream iss(line);
			iss >> word;
			if (word == "on") instruction.state = true;
			else if (word == "off") instruction.state = false;
			iss.ignore(3);
			iss >> instruction.bounds.min.x;
			iss.ignore(2);
			iss >> instruction.bounds.max.x;
			iss.ignore(3);
			iss >> instruction.bounds.min.y;
			iss.ignore(2);
			iss >> instruction.bounds.max.y;
			iss.ignore(3);
			iss >> instruction.bounds.min.z;
			iss.ignore(2);
			iss >> instruction.bounds.max.z;
			instructions.push_back(instruction);
		}
		inputStream.close();

		std::vector<Cuboid> active;
		std::vector<Cuboid> toAdd;
		for (auto instruction : instructions) {
			auto& bounds = instruction.bounds;
			if (bounds.max.x < -50 || bounds.min.x > 50 ||
				bounds.max.y < -50 || bounds.min.y > 50 ||
				bounds.max.z < -50 || bounds.min.z > 50)
				continue;

			if (bounds.max.x > 50) bounds.max.x = 50;
			if (bounds.max.y > 50) bounds.max.y = 50;
			if (bounds.max.z > 50) bounds.max.z = 50;
			if (bounds.min.x < -50) bounds.min.x = -50;
			if (bounds.min.y < -50) bounds.min.y = -50;
			if (bounds.min.z < -50) bounds.min.z = -50;
			toAdd.clear();
			for (int i=active.size()-1; i >= 0; i--) {
				if (exclude(active[i], bounds, toAdd)) {
					active.erase(active.begin() + i);
				}
			}
			for (auto& cuboid : toAdd) {
				active.push_back(cuboid);
			}
			if (instruction.state) active.push_back(bounds);
		}

		long long size = 0;
		for (auto& cuboid : active) {
			size += cuboid.size();
		}
		std::cout << size << std::endl;
	}

	static bool exclude(Cuboid cube, const Cuboid& mask, std::vector<Cuboid>& toAdd) {

		if (mask.max.x < cube.min.x || cube.max.x < mask.min.x ||
		mask.max.y < cube.min.y || cube.max.y < mask.min.y ||
		mask.max.z < cube.min.z || cube.max.z < mask.min.z) {
			return false;
		}

		ivec3 dMax = cube.max - mask.max;
		ivec3 dMin = mask.min - cube.min;

		if (dMin.x > 0) {
			toAdd.push_back(Cuboid{
		ivec3{cube.min.x, cube.min.y, cube.min.z},
		ivec3{cube.min.x + dMin.x - 1, cube.max.y, cube.max.z}});
			cube.min.x = mask.min.x;
		}
		if (dMax.x > 0) {
			toAdd.push_back(Cuboid{
		ivec3{cube.max.x - dMax.x + 1, cube.min.y, cube.min.z},
		ivec3{cube.max.x, cube.max.y, cube.max.z}});
			cube.max.x = mask.max.x;
		}
		if (dMin.y > 0) {
			toAdd.push_back(Cuboid{
			ivec3{cube.min.x, cube.min.y, cube.min.z},
			ivec3{cube.max.x, cube.min.y + dMin.y - 1, cube.max.z}});
			cube.min.y = mask.min.y;
		}
		if (dMax.y > 0) {
			toAdd.push_back(Cuboid{
			ivec3{cube.min.x, cube.max.y - dMax.y + 1, cube.min.z},
			ivec3{cube.max.x, cube.max.y, cube.max.z}});
			cube.max.y = mask.max.y;
		}
		if (dMin.z > 0) {
			toAdd.push_back(Cuboid{
			ivec3{cube.min.x, cube.min.y, cube.min.z},
			ivec3{cube.max.x, cube.max.y, cube.min.z + dMin.z - 1}});
			cube.min.z = mask.min.z;
		}
		if (dMax.z > 0) {
			toAdd.push_back(Cuboid{
			ivec3{cube.min.x, cube.min.y, cube.max.z - dMax.z + 1},
			ivec3{cube.max.x, cube.max.y, cube.max.z}});
			cube.max.z = mask.max.z;
		}
		return true;
	}
	static void runPart2() {
		std::vector<Cuboid> active;
		std::vector<Cuboid> toAdd;
		for (auto& instruction : instructions) {
			toAdd.clear();
			for (int i=active.size()-1; i >= 0; i--) {
				if (exclude(active[i], instruction.bounds, toAdd)) {
					active.erase(active.begin() + i);
				}
			}
			for (auto& cuboid : toAdd) {
				active.push_back(cuboid);
			}
			if (instruction.state) active.push_back(instruction.bounds);
		}

		long long size = 0;
		for (auto& cuboid : active) {
			size += cuboid.size();
		}
		std::cout << size << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

*/