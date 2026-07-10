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
#include "utils.h"

//moving through scanners

struct Scanner {
	int id;
	int size;
	int currentPos = 0;
	bool backDir;
	void Advance() {
		currentPos += backDir ? -1 : 1;
		if (currentPos == size - 1) backDir = true;
		if (currentPos == 0) backDir = false;
	}
	int GetPos(int t) {
		int last = size - 1;
		int phase = t % (last * 2);
		return (phase < last) ? phase : last - (phase - size - 1);
	}
};
struct Task
{
	static inline std::vector<Scanner> scanners;
	static void run() {

		std::ifstream inputStream("2017/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int id;
		int size;
		while (inputStream >> id)
		{
			inputStream.ignore(1);
			inputStream >> size;
			scanners.emplace_back(id, size,0,0);
		}
		inputStream.close();
		int score = 0;
		for (auto & scanner : scanners) {
			if (scanner.GetPos(scanner.id) == 0) {
				score += scanner.size * scanner.id;
			}
		}
		std::cout << score << std::endl;
	}
	static void runPart2() {
		int offset = 0;
		bool caught = false;
		do {
			caught = false;
			for (auto & scanner : scanners) {
				if (scanner.GetPos(scanner.id + offset) == 0) {
					caught = true;
					break;
				}
			}
			offset++;
		} while (caught);
		std::cout << offset - 1 << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/