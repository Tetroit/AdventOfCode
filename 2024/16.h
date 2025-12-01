#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <map>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

struct Task {

	enum struct Facing : uint8_t{
		EAST = 0x0,
		NORTH = 0x1,
		WEST = 0x2,
		SOUTH = 0x3,
	};

	const std::unordered_map<Facing, std::pair<int,int>> facingVec {
		{Facing::EAST, {1, 0}},
		{Facing::NORTH, {0, -1}},
		{Facing::WEST, {-1, 0}},
		{Facing::SOUTH, {0, 1}},
	};

	std::vector<std::tuple<int, int, int, Facing>>::const_iterator findPlace(const std::vector<std::tuple<int, int, int, Facing>>& arr, int order) {
		for ( int i=0; i<arr.size(); i++ ) {
			if (order < std::get<0>(arr.at(i))) {
				return arr.cbegin() + i;
			}
		}
		return arr.end();
	}


	int countTiles (int** weights, const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY) {

		std::vector<std::vector<bool>> counted;

		for (int i=0; i<height; i++) {
			counted.emplace_back();
			for (int j=0; j<width; j++) {
				counted[i].emplace_back(false);
			}
		}

		std::vector<std::tuple<int, int, Facing>> queue;
		std::vector<std::tuple<int, int, Facing>> toAdd;
		counted[endY][endX] = true;
		queue.emplace_back(endX, endY, Facing::EAST);

		while (!queue.empty()) {
			auto& [x, y, fac] = queue.front();
			counted[y][x] = true;
			for (int i = 0; i < 4; i++) {
				auto nextFac = static_cast<Facing>(i);
				auto& [dx, dy] = facingVec.at(nextFac);

				if (weights[y + dy][x + dx] == -1)
					continue;

				int extra = 0;
				//while deducing we will bump into a wall because the deer may reach the same tile from different directions
				//but dijkstra doesn't account for rotation AFTER arriving,
				//so we need to leave margin for cases when deer goes straight through that cell by adding extra 1000 points
				if (nextFac == fac)
					extra = 1000;
				if (weights[y + dy][x + dx] < weights[y][x] + extra) {
					toAdd.emplace_back(x + dx, y + dy, nextFac);
				}
			}

			queue.erase(queue.begin());
			for (auto& i : toAdd) {
				queue.push_back(i);
			}
			toAdd.clear();
		}
		int cnt = 0;
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				if (counted[i][j]) {
					std::cout << 'O';
					cnt++;
				}
				else {
					std::cout << '.';
				}
			}
			std::cout << '\n';
		}
		return cnt;

	}

	int Dijkstra(int** weights, const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY, const Facing& startFacing = Facing::EAST) {

		//weight, x, y, facing
		std::vector<std::tuple<int, int, int, Facing>> queue;

		queue.emplace(findPlace(queue, 0), 0, startX, startY, startFacing);
		while (!queue.empty()) {
			auto data = queue.begin();
			int& score = std::get<0>(*data);
			int& x = std::get<1>(*data);
			int& y = std::get<2>(*data);
			std::vector<std::tuple<int, int, int, Facing>> toAdd;
			Facing& facing = std::get<3>(*data);

			for (int i=0; i<4; i++) {

				auto& [dx, dy] = facingVec.at(static_cast<Facing>(i));
				if (weights[y + dy][x + dx] == -1)
					continue;
				//dont do 180 turn
				if (((int)facing - i + 4) % 4 == 2) {
					continue;
				}

				int cost = 0;
				if ((int)facing == i) cost = 1;
				else cost = 1001;

				int weight = score + cost;
				if (weights[y + dy][x + dx] > weight) {
					weights[y + dy][x + dx] = weight;
					toAdd.emplace_back(weight, x + dx, y + dy, static_cast<Facing>(i));
				}
			}

			queue.erase(data);

			for (auto& t : toAdd) {
				int weight = std::get<0>(t);
				queue.emplace(findPlace(queue, weight), t);
			}
		}
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				std::cout<<weights[i][j]<<' ';
			}
			std::cout<<'\n';
		}
		std::cout<<'\n';
		return weights[endY][endX];
	}
	void run();
};

inline void Task::run() {


	std::ifstream inputStream("input2024-16.txt");

	if (!inputStream.is_open()) {
		std::cerr << "Failed to open file!\n";
	}

	std::vector <std::string> maze;
	int** weights;

	int width = 0;
	int height = 0;

	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;

	std::string line;


	while (std::getline(inputStream, line))
	{
		width = line.length();
		height++;
		maze.push_back(line);
	}

	weights = new int*[height]();
	for (int i=0; i<height; i++) {
		weights[i] = new int[width];
	}

	for (int y = 0; y < height; y++) {
		line = maze[y];
		for (int x=0; x < width; x++ ) {
			if (line[x] == 'S') {
				startX = x;
				startY = y;
				weights[y][x] = 0;
			}
			if (line[x] == 'E') {
				endX = x;
				endY = y;
				weights[y][x] = std::numeric_limits<int>::max();
			}
			if (line[x] == '.') {
				weights[y][x] = std::numeric_limits<int>::max();
			}
			if (line[x] == '#') {
				weights[y][x] = -1;
			}
		}
	}

	std::cout << Dijkstra(weights, width, height, startX, startY, endX, endY, Facing::EAST) << '\n';
	std::cout << countTiles(weights, width, height, startX, startY, endX, endY);

	//583

	for (int i=0; i<height; i++) {
		delete[] weights[i];
	}
	delete[] weights;
}
