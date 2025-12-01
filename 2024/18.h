#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

const int SIZE = 71;

struct Task
{

	const std::unordered_map<int, std::pair<int, int>> facingVec {
		{0, {1,0}},
		{1, {0,-1}},
		{2, {-1,0}},
		{3, {0,1}},
	};

	std::vector<std::tuple<int, int, int>>::const_iterator findPlace(const std::vector<std::tuple<int, int, int>>& arr, int order) {
		for ( int i=0; i<arr.size(); i++ ) {
			if (order < std::get<0>(arr.at(i))) {
				return arr.cbegin() + i;
			}
		}
		return arr.end();
	}

	template <int s>
	int Dijkstra(int weights[][s], const int& width, const int& height, std::vector<std::pair<int,int>> starts, const int& endX, const int& endY) {
		//weight, x, y
		std::vector<std::tuple<int, int, int>> queue;

		for (auto& [x,y] : starts) {
			queue.emplace(findPlace(queue, weights[x][y]), weights[x][y], x, y);
		}
		while (!queue.empty()) {
			auto data = queue.begin();
			int& score = std::get<0>(*data);
			int& x = std::get<1>(*data);
			int& y = std::get<2>(*data);
			std::vector<std::tuple<int, int, int>> toAdd;

			for (int i=0; i<4; i++) {

				auto& [dx, dy] = facingVec.at(i);

				int nx = x + dx, ny = y + dy;

				if (!inRange(width, height, nx, ny)) {
					continue;
				}

				if (weights[nx][ny] == -1)
					continue;

				int weight = score + 1;
				if (weights[nx][ny] > weight) {
					weights[nx][ny] = weight;
					toAdd.emplace_back(weight, nx, ny);
				}
			}

			queue.erase(data);

			for (auto& t : toAdd) {
				int weight = std::get<0>(t);
				queue.emplace(findPlace(queue, weight), t);
			}
		}

		return weights[endX][endY];
	}

	template <int s>
	int Dijkstra(int weights[][s], const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY) {

		weights[startX][startY] = 0;

		std::vector<std::pair<int,int>> starts {
			{startX, startY}
		};
		Dijkstra(weights, width, height, starts, endX, endY);

		return weights[endX][endY];
	}

	template <int s>
	void eraseAfter (int weights[][s], const int& width, const int& height, int& x, int& y, std::vector<std::pair<int,int>>& exits, int score) {

		if (weights[x][y] != -1)
			weights[x][y] = std::numeric_limits<int>::max();

		for (int i=0; i<4; i++) {

			auto& [dx, dy] = facingVec.at(i);

			int nx = x + dx, ny = y + dy;
			if (!inRange(width, height, nx, ny)) {
				continue;
			}

			if (weights[nx][ny] == -1 || weights[nx][ny] == std::numeric_limits<int>::max())
				continue;

			if (weights[nx][ny] > score) {
				eraseAfter(weights, width, height, nx, ny, exits, score);
			}
			if (weights[nx][ny] == score) {
				exits.emplace_back(nx, ny);
			}
		}
	}
	template <int s>
	void inputNextN (int weights[][s], const int& width, const int& height, const int& limit, std::ifstream& inputStream) {

		int x, y;
		for (int i = 0; i < limit; i++) {

			inputStream >> x;
			char comma;
			inputStream >> comma;
			inputStream >> y;
			weights[x][y] = -1;
		}
	}

	void run() {


		int weights[SIZE][SIZE];

		for (int x=0; x<SIZE; x++) {
			for (int y =0; y<SIZE; y++)  {
				weights[x][y] = std::numeric_limits<int>::max();
			}
		}


		std::ifstream inputStream("input2024-18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		inputNextN(weights, SIZE, SIZE, 1024, inputStream);

		int steps = Dijkstra<SIZE> (weights, SIZE, SIZE, 0, 0, SIZE - 1, SIZE - 1);

		std::cout << steps << '\n';

		int blockX, blockY;
		while (steps < std::numeric_limits<int>::max() && !inputStream.eof()) {

			char ch;
			inputStream >> blockX >> ch >> blockY;
			std::vector<std::pair<int, int>> exits;
			eraseAfter<SIZE>(weights, SIZE, SIZE, blockX, blockY, exits, weights[blockX][blockY]);

			//display
			// for (int x=0; x<SIZE; x++) {
			// 	for (int y=0; y<SIZE; y++) {
			// 		if (x == blockX && y == blockY) {
			// 			std::cout<<'O';
			// 		}
			// 		else if (weights[x][y] == -1) {
			// 			std::cout<<'#';
			// 		}
			// 		else if (weights[x][y] == std::numeric_limits<int>::max()) {
			// 			std::cout << '?';
			// 		}
			// 		else {
			// 			std::cout << '.';
			// 		}
			// 	}
			// 	std::cout << '\n';
			// }
			std::cout << '\n';
			weights[blockX][blockY] = -1;
			steps = Dijkstra<SIZE> (weights, SIZE, SIZE, exits, SIZE - 1, SIZE - 1);
		}
		std::cout << blockX << " " << blockY << '\n';
	}
};
