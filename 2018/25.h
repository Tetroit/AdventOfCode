#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"
#include "vec.h"

//4d manhattan distance separate graphs


struct Task
{
	static void run() {

		std::ifstream inputStream("2018/25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<ivec4> stars;
		std::unordered_map<int, std::vector<int>> connections;

		while (std::getline(inputStream, line))
		{
			std::vector<std::string> coords = Utils::split(line, ',');
			ivec4 vec {std::stoi(coords[0]), std::stoi(coords[1]),std::stoi(coords[2]),std::stoi(coords[3])};
			stars.push_back(vec);
		}
		inputStream.close();
		for (int i=0; i < stars.size() - 1; i++) {
			for (int j=i+1; j < stars.size(); j++) {
				if ((stars[i] - stars[j]).len() <= 3) {
					connections[i].push_back(j);
					connections[j].push_back(i);
				}
			}
		}
		std::vector<int> group(stars.size(), -1);
		int groupId = 0;
		for (int i=0; i<stars.size(); i++) {
			if (group[i] == -1) {
				std::deque<int> queue{i};
				group[i] = groupId;
				while (!queue.empty()) {
					int parent = queue.front();
					queue.pop_front();
					for (auto& id : connections[parent]) {
						if (group[id] != groupId) {
							group[id] = groupId;
							queue.push_back(id);
						}
					}
				}
				groupId++;
			}
		}
		std::cout << groupId << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/