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
	void countFence(const std::vector <std::string>& land, bool** flags, int width, int height, std::vector<int>& pers, std::vector<int>& areas) {

		int regions = 0;
		for (int y=0; y<height; y++) {
			for (int x = 0; x < width; x++) {
				if (!flags[y][x]) {
					regions++;
					pers.emplace_back(0);
					areas.emplace_back(0);
					int& per = pers.back();
					int& area = areas.back();
					//countRegion(land, flags, width, height, x, y, per, area);
					countRegionDiscounted(land, flags, width, height, x, y, per, area);
				}
			}
		}

	}

	const std::pair<int,int> deltas[4] = {
	{1,0},
	{-1,0},
	{0,1},
	{0,-1}
	};

	void countRegion (const std::vector <std::string>& land, bool** flags, int width, int height, int x, int y, int& per, int& area) {
		flags[y][x] = true;
		area++;
		for (const auto& pair : deltas) {

			int nx = x + pair.first;
			int ny = y + pair.second;
			if (!inRange(width, height, nx, ny)) {
				per++;
				continue;
			}
			if (land[y][x] != land[ny][nx]) {
				per++;
				continue;
			}
			if (!flags[ny][nx])
				countRegion(land, flags, width, height, nx, ny, per, area);
		}
	}

	void rotate(int& dx, int& dy) {
		int temp = dx;
		dx = dy;
		dy = -temp;
	}

	bool borderAlreadyCounted (const std::vector <std::string>& land, bool** flags, int width, int height, int x, int y, int dx, int dy) {
		int dirx = dx;
		int diry = dy;
		rotate(dirx, diry);
		int currentX;
		int currentY;

		bool isVoid = false;
		if (!inRange(width, height, x + dx, y + dy)) {
			isVoid = true;
		}

		auto isPartOfBorder = [&]() {
			if (!inRange(width, height, currentX, currentY))	return false;
			if (land[currentY][currentX] != land[y][x])			return false;
			if (isVoid)											return true;

			return land[currentY + dy][currentX + dx] != land[y][x];
		};

		currentX = x + dirx;
		currentY = y + diry;

		//one direction
		while (isPartOfBorder()) {
			if (flags[currentY][currentX]) {
				return true;
			}
			currentX += dirx;
			currentY += diry;
		}

		currentX = x - dirx;
		currentY = y - diry;

		//other direction
		while (isPartOfBorder()) {
			if (flags[currentY][currentX]) {
				return true;
			}
			currentY -= diry;
			currentX -= dirx;
		}
		return false;

	}
	void countRegionDiscounted (const std::vector <std::string>& land, bool** flags, int width, int height, int x, int y, int& per, int& area) {
		flags[y][x] = true;
		area++;
		for (const auto& pair : deltas) {
			int nx = x + pair.first;
			int ny = y + pair.second;
			if (!inRange(width, height, nx, ny)) {
				if (!borderAlreadyCounted(land, flags, width, height, x, y, pair.first, pair.second))
					per++;
				continue;
			}
			if (land[y][x] != land[ny][nx]) {
				if (!borderAlreadyCounted(land, flags, width, height, x, y, pair.first, pair.second))
					per++;
			}
		}
		for (const auto& pair : deltas)
		{
			int nx = x + pair.first;
			int ny = y + pair.second;
			if (inRange(width, height, nx, ny) && !flags[ny][nx] && land[y][x] == land[ny][nx])
				countRegionDiscounted(land, flags, width, height, nx, ny, per, area);
		}
	}

	void run() {

		std::ifstream inputStream("input2024-12.txt");


		std::vector <std::string> land;
		bool** flags;
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::string line;
		while (std::getline( inputStream, line ))
		{
			land.push_back(line);
		}

		const int height = land.size();
		const int width = land[0].length();

		flags = new bool* [height];
		for (int i=0; i<height; i++) {
			flags[i] = new bool[width];
			for (int j=0; j<width; j++) {
				flags[i][j] = false;
			}
		}

		int score = 0;
		std::vector<int> pers;
		std::vector<int> areas;
		countFence(land, flags, width, height, pers, areas);

		for (int i = 0; i < pers.size(); i++) {
			score += pers[i] * areas[i];
		}

		std::cout << score << std::endl;


		for (int i=0; i<height; i++) {
			delete flags[i];
		}
		delete flags;
	}
};
