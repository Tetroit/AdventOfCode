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

//checking grid nodes in folder


struct NodeData {
	int total;
	int taken;
	[[nodiscard]] int available() const {
		return total - taken;
	}
};
struct Task
{
	static inline const std::regex pattern = std::regex(
		R"(\/dev\/grid\/node-x(\d+)-y(\d+) +(\d+)T +(\d+)T +(\d+)T +(\d+)%)", std::regex::optimize);
	static inline DynamicGrid<NodeData> nodes;
	static void run() {

		std::ifstream inputStream("2016/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::smatch match;

		while (std::getline(inputStream, line))
		{
			if (std::regex_match(line, match, pattern)) {
				NodeData data{};
				int x = std::stoi(match[1].str());
				int y = std::stoi(match[2].str());
				data.total = std::stoi(match[3].str());
				data.taken = std::stoi(match[4].str());
				nodes.add(x,y,data);
			}
		}
		inputStream.close();
		int cnt = 0;
		for (int y1 = 0; y1 < nodes.getHeight(); y1++) {
			for (int x1 = 0; x1 < nodes.getWidth(); x1++) {
				for (int y2 = 0; y2 < nodes.getHeight(); y2++) {
					for (int x2 = 0; x2 < nodes.getWidth(); x2++) {
						if (x1 == x2 && y1 == y2) continue;
						const auto& n1 = nodes.get(x1, y1);
						const auto& n2 = nodes.get(x2, y2);
						if (n1.taken == 0) continue;
						if (n1.taken < n2.available()) {
							cnt++;
						}
					}
				}
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		DynamicGrid<int> path;
		int startX, startY;
		path.resize(nodes.getWidth(), nodes.getHeight());
		nodes.foreachCoord([&](const int& x, const int& y) {
			int load = nodes.get(x, y).taken;
			int id;
			if (load == 0) {
				startX = x;
				startY = y;
				id = 0;
			}
			else if (load > 100) id = 2;
			else id = 1;
			path.set(x, y, id);
		});
		path.print([](const int& i) {
			if (i==0) return '_';
			else if (i==1) return '.';
			else return '#';
		});
	}
};

//-------------- NOTES AREA ----------------
/*
	seems like we can freely move data except for big nodes as most nodes have
	80-99 total space
	60-79 taken space

	there is a single wall, so the res is
	17 move to the left wall
	22 move to the top wall
	35 move to the right wall (goal moves 1 cell left)
	34 * 5 because it takes 5 moves to push result in 1 direction
	total = 244
*/