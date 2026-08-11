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

//finding the closest wire crossing
struct Task
{
	static void run() {

		std::ifstream inputStream("2019/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::unordered_set<ivec2, ivec2hash> wire1;
		std::string line;
		std::getline(inputStream, line);
		char dir;
		int num;

		{
			std::istringstream iss(line);
			ivec2 pos{0,0};
			while (iss >> dir >> num)
			{
				iss.ignore(1);
				ivec2 inc{0,0};
				switch (dir) {
					case 'R': inc.x = 1; break;
					case 'L': inc.x = -1; break;
					case 'D': inc.y = 1; break;
					case 'U': inc.y = -1; break;
				}
				for (int i=0; i<num; i++) {
					pos+=inc;
					wire1.insert(pos);
				}
			}
		}

		std::getline(inputStream, line);
		std::unordered_set<ivec2, ivec2hash> intersections;

		{
			std::istringstream iss(line);
			ivec2 pos{0,0};
			while (iss >> dir >> num)
			{
				iss.ignore(1);
				ivec2 inc{0,0};
				switch (dir) {
					case 'R': inc.x = 1; break;
					case 'L': inc.x = -1; break;
					case 'D': inc.y = 1; break;
					case 'U': inc.y = -1; break;
				}
				for (int i=0; i<num; i++) {
					pos+=inc;
					if (wire1.contains(pos))
						intersections.insert(pos);
				}
			}
		}
		inputStream.close();
		int closest = INT_MAX;
		for (auto& intersection : intersections) {
			int dist = intersection.len();
			if (dist < closest) closest = dist;
		}
		std::cout << closest << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2019/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::unordered_map<ivec2, int, ivec2hash> wire1;
		std::string line;
		std::getline(inputStream, line);
		char dir;
		int num;

		{
			std::istringstream iss(line);
			ivec2 pos{0,0};
			int dist = 0;
			while (iss >> dir >> num)
			{
				iss.ignore(1);
				ivec2 inc{0,0};
				switch (dir) {
					case 'R': inc.x = 1; break;
					case 'L': inc.x = -1; break;
					case 'D': inc.y = 1; break;
					case 'U': inc.y = -1; break;
				}
				for (int i=0; i<num; i++) {
					pos+=inc;
					dist++;
					wire1.try_emplace(pos, dist);
				}
			}
		}

		std::getline(inputStream, line);
		std::unordered_map<ivec2, ivec2, ivec2hash> intersections;
		{
			std::istringstream iss(line);
			ivec2 pos{0,0};
			int dist = 0;
			while (iss >> dir >> num)
			{
				iss.ignore(1);
				ivec2 inc{0,0};
				switch (dir) {
					case 'R': inc.x = 1; break;
					case 'L': inc.x = -1; break;
					case 'D': inc.y = 1; break;
					case 'U': inc.y = -1; break;
				}
				for (int i=0; i<num; i++) {
					pos+=inc;
					dist++;
					if (wire1.contains(pos))
						intersections.try_emplace(pos, wire1.at(pos), dist);
				}
			}
		}
		inputStream.close();
		int closest = INT_MAX;
		for (auto& [intersection, bothDist] : intersections) {
			int dist = bothDist.len();
			if (dist < closest) closest = dist;
		}
		std::cout << closest << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/