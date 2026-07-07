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

//roomba

struct Task
{
	static inline DynamicGrid<int> grid;
	static inline DynamicGrid<int> adjMat;
	static void run() {

		std::ifstream inputStream("2016/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int lineN = 0;
		int startX, startY;
		std::unordered_map<int, std::pair<int, int>> points;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(lineN, line, [](char ch) {
				if (ch == '#') return -1;
				return 0;
			});
			for (int i=0; i<line.length(); i++) {
				char ch = line[i];
				if (ch >= '0' && ch <= '9')
					points[ch - '0'] = std::make_pair(i, lineN);
			}
			lineN++;
		}
		inputStream.close();
		int start = 0;
		int path = 0;
		for (int i=0; i<points.size() - 1; i++) {
			DynamicGrid<int> weights = grid.dijkstra(points[i].first, points[i].second,
					[](int _){return 1;}, [](int x){return x<0;});
			for (int j=i+1; j<points.size(); j++) {
				//I can definitely check distances to all other points in one dijkstra dispatch but im lazy haha
				adjMat.add(i, j, weights.get(points[j].first, points[j].second) - 1);
				adjMat.add(j, i, weights.get(points[j].first, points[j].second) - 1);
			}
		}
		std::vector<int> order;
		for (int i=1; i<points.size(); i++) {
			order.push_back(i);
		}
		int min = std::numeric_limits<int>::max();
		while (std::next_permutation(order.begin(), order.end())) {
			int sum = adjMat.get(0, order[0]);
			for (int i=0; i<order.size()-1; i++) {
				sum += adjMat.get(order[i], order[i+1]);
			}
			if (sum < min) {
				min = sum;
			}
		}
		std::cout << min << std::endl;
	}
	static void runPart2() {
		std::vector<int> order;
		for (int i=1; i<adjMat.getWidth(); i++) {
			order.push_back(i);
		}
		int min = std::numeric_limits<int>::max();
		while (std::next_permutation(order.begin(), order.end())) {
			int sum = adjMat.get(0, order[0]) + adjMat.get(0, order[order.size()-1]);
			for (int i=0; i<order.size()-1; i++) {
				sum += adjMat.get(order[i], order[i+1]);
			}
			if (sum < min) {
				min = sum;
			}
		}
		std::cout << min << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/