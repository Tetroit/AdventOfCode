#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

struct Task
{


	bool isTower(char ch) {
		if (ch >= '0' && ch <= '9')
		{
			return true;
		}
		if (ch >= 'a' && ch <= 'z')
		{
			return true;
		}
		if (ch >= 'A' && ch <= 'Z')
		{
			return true;
		}
		return false;
	}
	struct pos {
		int x;
		int y;
		pos(int x, int y) : x(x), y(y){}

		bool operator==(const pos& other) const {
			return x == other.x && y == other.y;
		}
		pos operator + (const pos& other) const {
			return pos(x + other.x, y + other.y);
		}
		pos operator - (const pos& other) const {
			return pos(x - other.x, y - other.y);
		}
		// pos warped(int w, int h)
		// {
		// 	int modx = x % w;
		// 	int mody = y % h;
		// 	if (modx < 0)
		// 		modx += w;
		// 	if (mody < 0)
		// 		mody += h;
		// 	return pos(modx, mody);
		// }
		bool inRange(int w, int h) {
			if (x >= 0 && x < w && y >= 0 && y < h)
				return true;
			return false;
		}
	};
	struct posHasher {
		size_t operator()(const pos& p) const noexcept {
			return std::hash<int>()(p.x)
				^ (std::hash<int>()(p.y) << 1);
		}
	};


	void mapAntis(std::vector<std::string>& field, const std::unordered_set<pos, posHasher>& antis, int width, int height) {
		for (auto& anti : antis) {
			field[anti.y][anti.x] = '#';
		}
		//field display
		for (int y = 0; y < height; y++)
		{
			const std::string& line = field.at(y);
			for (int x = 0; x < width; x++)
			{
				std::cout << line[x];
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	void run() {

		std::vector<std::string> field;
		std::unordered_map<char, std::vector<pos>> towers;
		std::unordered_set<pos, posHasher> antis;

		std::string in;
		std::ifstream inputStream("input2024-08.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		int width, height;
		while (std::getline(inputStream, in))
		{
			field.push_back(in);
		}
		width = field[0].size();
		height = field.size();

		for (int y = 0; y < height; y++)
		{
			const std::string& line = field.at(y);
			for (int x = 0; x < width; x++)
			{
				if (isTower(line[x])) {
					auto res = towers.try_emplace(line[x], std::vector<pos>());
					res.first->second.emplace_back(x, y);
				}
			}
		}
		for (auto& pair : towers) {
			auto& arr = pair.second;
			if (arr.size() < 2)
				continue;
			for (int i = 0; i < arr.size() - 1; i++)
			{
				for (int j = i + 1; j < arr.size(); j++)
				{
					pos diff = arr[i] - arr[j];
					pos pos1 = arr[i];
					pos pos2 = arr[j];
					while (pos1.inRange(width, height)) {
						antis.insert(pos1);
						pos1 = pos1 + diff;
					}

					while (pos2.inRange(width, height)) {
						antis.insert(pos2);
						pos2 = pos2 - diff;

					}
				}
			}
		}
		mapAntis(field, antis, width, height);

		std::cout << antis.size() << '\n';
	}
};