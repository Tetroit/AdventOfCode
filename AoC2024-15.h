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

struct Task {
	int GPSScore(int x, int y) {
		return 100 * y + x;
	}
	int totalGPSScore(const std::vector<std::string>& field, int width, int height) {
		int res = 0;
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				if (field[y][x] == 'O' || field[y][x] == '[')
					res += GPSScore(x, y);
			}
		}
		return res;
	}
	const std::unordered_map<char, std::pair<int,int>> dirs{
			{'>', {1, 0}},
			{'<', {-1, 0}},
			{'^', {0, -1}},
			{'v', {0, 1}},
		};

	// bool searchFreeSpace(const std::vector<std::string>& field, int width, int height, int& x, int& y, int dx, int dy) {
	//
	// 	while (inRange(width, height, x, y) && field[y][x] != '#') {
	// 		if (field[y][x] == '.') return true;
	// 		x += dx;
	// 		y += dy;
	// 	}
	// 	return false;
	// }

	bool push (std::vector<std::string>& field, int width, int height, int x, int y, int dx, int dy) {
		if (!inRange(width, height,x,y))
			return false;

		int nx = x + dx;
		int ny = y + dy;
		if (field[ny][nx] == '#')
			return false;
		if (field[ny][nx] == 'O') {
			bool success = push(field, width, height, nx, ny, dx, dy);
			if (success)
				std::swap(field[y][x], field[ny][nx]);
			return success;
		}
		if (field[ny][nx] == '.') {
			std::swap(field[y][x], field[ny][nx]);
			return true;
		}
	}

	void undoDouble(std::vector<std::string>& field, int width, int height, int x, int y, int dx, int dy) {
		int ox = x - dx;
		int oy = y - dy;

		field[oy][ox] = '['; field[oy][ox + 1] = ']';
		field[y][x] = '.'; field[y][x + 1] = '.';
	}
	//takes position of the '[' cell!!
	bool pushDouble(std::vector<std::string>& field, int width, int height, int x, int y, int dx, int dy, std::vector<std::pair<int, int>>* pushStack = nullptr) {
		if (!inRange(width, height,x,y))
			return false;


		//horizontal movement
		if (dx == 1 && dy == 0) {
			if (field[y][x + 2] == '#')
				return false;

			if (field[y][x + 2] == '[')
				pushDouble(field, width, height, x + 2, y, dx, dy);

			if (field[y][x + 2] == '.') {
				field[y][x] = '.';
				field[y][x + 1] = '[';
				field[y][x + 2] = ']';
				return true;
			}
			return false;
		}

		if (dx == -1 && dy == 0) {
			if (field[y][x - 1] == '#')
				return false;

			if (field[y][x - 1] == ']')
				pushDouble(field, width, height, x - 2, y, dx, dy);

			if (field[y][x - 1] == '.') {
				field[y][x - 1] = '[';
				field[y][x] = ']';
				field[y][x + 1] = '.';
				return true;
			}
			return false;
		}

		//vertical movement
		int nx = x + dx;
		int ny = y + dy;
		if (field[ny][nx] == '#' || field[ny][nx + 1] == '#')
			return false;
		if (field[ny][nx] == '[') {
			pushDouble(field, width, height, nx, ny, dx, dy, pushStack);
		}

		//this is so bad, if one fails the other one must be reverted
		//and there's no good method to do so with my approach...
		//so FUCK IT, I WILL TRACK CHANGES MANUALLY MUAHAHAHAH

		bool startedBranch = false;

		if (field[ny][nx] == ']' && field[ny][nx + 1] == '[') {
			if (pushStack == nullptr) {
				pushStack = new std::vector<std::pair<int, int>>();
				startedBranch = true;
			}

			if (field[ny][nx] == ']') {
				pushDouble(field, width, height, nx - 1, ny, dx, dy, pushStack);

			}
			if (field[ny][nx + 1] == '[') {
				pushDouble(field, width, height, nx + 1, ny, dx, dy, pushStack);
			}
		}
		else {
			if (field[ny][nx] == ']') {
				pushDouble(field, width, height, nx - 1, ny, dx, dy, pushStack);
			}
			if (field[ny][nx + 1] == '[') {
				pushDouble(field, width, height, nx + 1, ny, dx, dy, pushStack);
			}
		}

		//evaluate result
		if (field[ny][nx] == '.' && field[ny][nx + 1] == '.') {
			field[ny][nx] = '['; field[ny][nx + 1] = ']';
			field[y][x] = '.'; field[y][x + 1] = '.';

			if (startedBranch && pushStack != nullptr) {
				pushStack->clear();
				delete pushStack;
				pushStack = nullptr;
			}
			else if (pushStack != nullptr) {
				pushStack->emplace_back(nx, ny);
			}
			return true;
		}

		if (startedBranch) {
			if (pushStack->size() > 0) {
				for (int i = pushStack->size() - 1; i >= 0; i--) {
					auto& pair = pushStack->at(i);
					undoDouble(field, width, height, pair.first, pair.second, dx, dy);
				}
				pushStack->clear();
			}
			delete pushStack;
			pushStack = nullptr;
		}

		return false;
	}


	std::vector<std::string> makeWide (const std::vector<std::string>& field) {

		auto res = std::vector<std::string>();

		for (int y=0; y<field.size(); y++) {
			res.emplace_back();
			for (int x = 0; x < field[y].size(); x++) {
				if (field[y][x] == 'O')
					res[y] += "[]";
				if (field[y][x] == '.')
					res[y] += "..";
				if (field[y][x] == '#')
					res[y] += "##";
				if (field[y][x] == '@')
					res[y] += "@.";
			}
		}
		return res;
	}
	void simulate (std::vector<std::string>& field, int width, int height, int& x, int& y, const std::string& instructions, int& instructionsId) {
		auto command = instructions[instructionsId];
		instructionsId++;
		auto dir = dirs.at(command);
		auto dx = dir.first;
		auto dy = dir.second;
		auto nx = x + dx;
		auto ny = y + dy;
		if (!inRange(width, height, nx, ny))
			return;
		if (field[ny][nx] == '#') {
			return;
		}
		if (field[ny][nx] == 'O') {
			if (push(field, width, height, nx, ny, dx, dy)) {
				std::swap(field[y][x], field[ny][nx]);
				x = nx;
				y = ny;
			}
			return;
		}
		//needed for part 2
		if (field[ny][nx] == '[') {
			if (pushDouble(field, width, height, nx, ny, dx, dy)) {
				std::swap(field[y][x], field[ny][nx]);
				x = nx;
				y = ny;
			}
			return;
		}
		//needed for part 2
		if (field[ny][nx] == ']') {
			if (pushDouble(field, width, height, nx - 1, ny, dx, dy)) {
				std::swap(field[y][x], field[ny][nx]);
				x = nx;
				y = ny;
			}
			return;
		}
		if (field[ny][nx] == '.') {
			std::swap(field[ny][nx], field[y][x]);
			x = nx;
			y = ny;
		}
	}

	void run() {


		std::ifstream inputStream("input2024-15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::vector <std::string> storage;
		std::string instructions;
		int width = 0;
		int height = 0;

		std::string line;
		bool fieldInput = true;
		int robotX = 0;
		int robotY = 0;
		while (std::getline(inputStream, line))
		{
			if (line == "") {

				fieldInput = false;
				continue;
			}

			if (fieldInput) {
				width = line.length();
				height++;
				storage.push_back(line);
				for (int i=0; i < width; i++ ) {
					if (line[i] == '@') {
						robotX = i;
						robotY = height - 1;
					}
				}
			}
			else {
				instructions += line;
			}
		}

		auto wide = makeWide(storage);
		int robotXWide = robotX * 2;
		int robotYWide = robotY;

		int instructionID = 0;
		// while (instructionID < instructions.size()) {
		// 	simulate(storage, width, height, robotX, robotY, instructions, instructionID);
		//
		// 	std::cout << instructionID << '\n';
		// 	if (instructions.size() - instructionID > 100)
		// 		std::cout << "..." << '\n';
		// 	else {
		// 		for (int y = 0; y < height; ++y) {
		// 			for (int x =0; x < width; ++x) {
		// 				std::cout << storage[y][x];
		// 			}
		// 			std::cout << '\n';
		// 		}
		// 	}
		// 	std::cout << '\n';
		// }

		instructionID = 0;
		while (instructionID < instructions.size()) {
			simulate(wide, width * 2, height, robotXWide, robotYWide, instructions, instructionID);

			std::cout << instructionID << '\n';
			if (instructions.size() - instructionID > 100)
				std::cout << "..." << '\n';
			else {
				for (int y = 0; y < height; ++y) {
					for (int x =0; x < width * 2; ++x) {
						std::cout << wide[y][x];
					}
					std::cout << '\n';
				}
			}
			std::cout << '\n';
		}
		std::cout << totalGPSScore(wide, width * 2, height) << '\n';

		//1458089
		//1432781
	}
};
