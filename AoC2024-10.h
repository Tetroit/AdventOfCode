#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{
	int traverse(std::vector<std::string>& field, int width, int height, int x, int y, int* cnt = nullptr) {
		std::unordered_set<int> ends;
		traverse(field, ends, width, height, x, y, '0', cnt);
		return ends.size();
	}
	void traverse(const std::vector<std::string>& field, std::unordered_set<int>& ends, int width, int height, int x, int y, char current, int* count = nullptr) {
		if (current == '9') {
			ends.insert(x + y * width);
			if (count != nullptr) {
				(*count)++;
			}
		}
		current++;
		if (inRange(width, height, x + 1, y) && field[x+1][y] == current) {
			traverse(field, ends, width, height, x + 1, y, current, count);
		}
		if (inRange(width, height, x, y + 1) && field[x][y + 1] == current) {
			traverse(field, ends, width, height, x, y + 1, current, count);
		}
		if (inRange(width, height, x - 1, y) && field[x - 1][y] == current) {
			traverse(field, ends, width, height, x - 1, y, current, count);
		}
		if (inRange(width, height, x, y - 1) && field[x][y - 1] == current) {
			traverse(field, ends, width, height, x, y - 1, current, count);
		}
	}
	void run() {

		std::ifstream inputStream("input2024-10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::pair<int, int>> starts;
		std::vector<std::string> field;
		std::string line;
		while (std::getline(inputStream, line))
		{
			field.push_back(line);
		}

		int width = field[0].length();
		int height = field.size();

		for (int y =0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (field[y][x] == '0') {
					starts.emplace_back(y, x);
				}
			}
		}
		int sum = 0;
		int trails = 0;
		for (auto& pair : starts) {
			sum += traverse(field, width, height, pair.first, pair.second, &trails);
		}

		std::cout << sum << '\n';
		std::cout << trails << '\n';
	}
};
