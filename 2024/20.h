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

const int CHEAT_THRESHOLD = 100;
const int CHEAT_LENGTH = 20;

struct Task
{

	struct Cheat {
		int startX;
		int startY;
		int endX;
		int endY;
		int time;
	};

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

	void SearchShortcuts(const std::vector<std::vector<int>>& weights, const int& width, const int& height, const int& currentX, const int& currentY, std::vector<Cheat>& cheats, int threshold) {
		for (int dx = - CHEAT_LENGTH; dx <= + CHEAT_LENGTH; dx++) {
			for (int dy = - CHEAT_LENGTH; dy <= + CHEAT_LENGTH; dy++) {
				int cost = std::abs(dx) + std::abs(dy);
				if (cost > CHEAT_LENGTH)
					continue;
				int resX = currentX + dx;
				int resY = currentY + dy;
				if (!inRange(width, height, resX, resY))
					continue;
				if (weights[resX][resY] == -1)
					continue;
				if (weights[resX][resY] < weights[currentX][currentY] - cost) {

					int skip = weights[currentX][currentY] - weights[resX][resY] - cost;
					if (skip >= threshold)
						cheats.push_back({resX, resY, currentX, currentY, skip});
				}
			}
		}
	}
	int Dijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, std::vector<std::pair<int,int>> starts, const int& endX, const int& endY, std::vector<Cheat>& cheats) {
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

			SearchShortcuts(weights, width, height, x, y, cheats, CHEAT_THRESHOLD);
			for (int i=0; i<4; i++) {

				auto& [dx, dy] = facingVec.at(i);

				int nx = x + dx, ny = y + dy;

				if (!inRange(width, height, nx, ny)) {
					continue;
				}

				if (weights[nx][ny] == -1) {
					// if (inRange(width, height, nx + dx, ny + dy)) {
					// 	int cheatX = nx + dx;
					// 	int cheatY = ny + dy;
					// 	if (weights[cheatX][cheatY] != -1 && weights[cheatX][cheatY] < weights[x][y] - 2) {
					// 		int diff = weights[x][y] - 2 - weights[cheatX][cheatY];
					// 		cheats.push_back({nx + dx, ny + dy, x, y, diff});
					// 	}
					// }
					continue;
				}

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
	int Dijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY, std::vector<Cheat>& cheats) {

		weights[startX][startY] = 0;

		std::vector<std::pair<int,int>> starts {
				{startX, startY}
		};
		Dijkstra(weights, width, height, starts, endX, endY, cheats);

		return weights[endX][endY];
	}

	void run() {

		std::ifstream inputStream("input2024-20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::vector <std::string> track;
		std::vector <std::vector<int>> weights;
		std::string line;

		int height = 0;
		int width = 0;

		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;

		while (std::getline(inputStream, line))
		{
			if (auto pos = line.find('S'); pos != std::string::npos) {
				startX = (int)pos;
				startY = height;
			}
			if (auto pos = line.find('E'); pos != std::string::npos) {
				endX = (int)pos;
				endY = height;
			}
			track.push_back(line);
			height++;
		// }
		width = track[0].length();

		weights = std::vector(width, std::vector(height, 0));

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (track[y][x] == '.' || track[y][x] == 'S' || track[y][x] == 'E') {}
					weights[x][y] = std::numeric_limits<int>::max();
				if (track[y][x] == '#') {
					weights[x][y] = -1;
				}
			}
		}

		std::vector<Cheat> cheats;
		int dist = Dijkstra(weights, width, height, startX, startY, endX, endY, cheats);
		std::cout << dist << '\n';

		int cnt = 0;
		for (auto& t : cheats) {
			if (t.time >= CHEAT_THRESHOLD) {
				//std::cout << "Start: (" << t.startX << ";"<< t.startY << ")\nEnd: (" << t.endX << ";" <<  t.endY << ")\nTime: " << t.time << "\n";
				cnt++;
			}
		}

		std::cout << cnt << '\n';
	}
};
