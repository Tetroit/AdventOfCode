#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <variant>
#include <vector>
#include "utils.h"

//Binary tree partitioning

struct Task
{
	static inline std::vector<int> passes;
	static void run() {

		std::ifstream inputStream("2020/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int maxID = 0;
		while (std::getline(inputStream, line))
		{
			int row = 0;
			for (int i=0; i<line.size()-3; i++) {
				row <<= 1;
				if (line[i] == 'F') row |= 0;
				if (line[i] == 'B') row |= 1;
			}
			int col = 0;
			for (int i=line.size() - 3; i<line.size(); i++) {
				col <<= 1;
				if (line[i] == 'L') col |= 0;
				if (line[i] == 'R') col |= 1;
			}
			int id = row*8+col;
			passes.emplace_back(id);
			maxID = std::max(maxID, id);
		}
		inputStream.close();
		std::cout << maxID << std::endl;
	}
	static void runPart2() {
		std::sort(passes.begin(), passes.end());
		int prevPass = *passes.begin();
		for (int i=1; i<passes.size(); i++) {
			int val = passes[i];
			if (passes[i] - prevPass == 2) {
				std::cout << prevPass + 1 << std::endl;
			}
			prevPass = val;
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/