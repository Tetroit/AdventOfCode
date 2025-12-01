#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "../utils.h"

struct Task
{
	bool in_range(const std::vector<std::string>& table, int posX, int posY) {
		int sizeY = table.size();
		int sizeX = table[0].length();
		if (posX < 0 || posY < 0 || posX >= sizeX || posY >= sizeY) {
			return false;
		}
		return true;
	}
	bool check_x(const std::vector<std::string>& table, int posX, int posY) {

		int sizeY = table.size();
		int sizeX = table[0].length();
		if (posX < 1 || posY < 1 || posX >= sizeX - 1 || posY >= sizeY - 1) {
			return false;
		}
		if (table[posY][posX] != 'A')
			return false;

		bool foundM = false;
		bool foundS = false;
		if (table[posY - 1][posX - 1] == 'M') foundM = true;
		if (table[posY - 1][posX - 1] == 'S') foundS = true;

		if (table[posY + 1][posX + 1] == 'M') foundM = true;
		if (table[posY + 1][posX + 1] == 'S') foundS = true;
		if (!(foundM && foundS)) {
			return false;
		}

		foundM = false;
		foundS = false;
		if (table[posY - 1][posX + 1] == 'M') foundM = true;
		if (table[posY - 1][posX + 1] == 'S') foundS = true;

		if (table[posY + 1][posX - 1] == 'M') foundM = true;
		if (table[posY + 1][posX - 1] == 'S') foundS = true;
		if (!(foundM && foundS)) {
			return false;
		}

		return true;

	}
	bool check_in_dir(const std::vector<std::string>& table, int deltaX, int deltaY, int posX, int posY, const std::string& word) {
		
		int wLength = word.length();

		if (!in_range(table, posX + deltaX * (wLength-1), posY + deltaY * (wLength-1))) {
			return false;
		}
		for (int i = 0; i < wLength; i++)
		{
			int itX = posX + deltaX * i;
			int itY = posY + deltaY * i;
			if (table[itY][itX] != word[i])
			{
				return false;
			}
		}
		return true;
	}
	void run() {

		std::ifstream inputStream("input2024-04.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::string> table;
		int sizeX = 0;
		int sizeY = 0;
		std::string input;
		while (std::getline(inputStream, input))
		{
			sizeX = input.length();
			table.push_back(input);
		}
		sizeY = table.size();
		int cnt = 0;
		int xmasCnt = 0;
		for (int x = 0; x < sizeY; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				if (check_in_dir(table, 1, -1, x, y, "XMAS")) cnt++;
				if (check_in_dir(table, 1, 0, x, y, "XMAS")) cnt++;
				if (check_in_dir(table, 1, 1, x, y, "XMAS")) cnt++;

				if (check_in_dir(table, -1, -1, x, y, "XMAS")) cnt++;
				if (check_in_dir(table, -1, 0, x, y, "XMAS")) cnt++;
				if (check_in_dir(table, -1, 1, x, y, "XMAS")) cnt++;

				if (check_in_dir(table, 0, -1, x, y, "XMAS")) cnt++;
				if (check_in_dir(table, 0, 1, x, y, "XMAS")) cnt++;

				//part 2

				if (check_x(table, x, y)) xmasCnt++;
			}
		}
		std::cout << cnt << "\n";
		std::cout << xmasCnt << "\n";
	}
};
