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

//The task about fitting presents inside boxes
struct Task
{
	struct TreeArea {
		int w;
		int h;
		std::vector<int> presents;
	};
	struct Present {
		std::vector<std::pair<int,int>> cells;
		[[nodiscard]] int Size() const {
			return cells.size();
		}
	};
	static void run() {
		std::ifstream inputStream("2025/input/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::string line;
		std::vector<Present> presents;
		std::vector<TreeArea> treeAreas;
		//input
		while (std::getline(inputStream, line))
		{
			if (line.empty())
				continue;

			int sep = line.find('x');
			if (sep != std::string::npos) {
				int w = std::stoi(line.substr(0, sep));
				int start = sep + 1;
				sep = line.find(':');
				int h = std::stoi(line.substr(start, sep - start));
				line = line.substr(sep + 2);
				auto words = Utils::split(line, ' ');
				std::vector<int> presentList;
				{
					for (auto& word : words) {
						presentList.push_back(std::stoi(word));
					}
				}
				treeAreas.push_back({w, h, presentList});
			}
			else {
				std::vector<std::pair<int,int>> grid;
				for (int i=0; i < 3; i++) {
					getline(inputStream, line);
					for (int j=0; j < 3; j++) {
						if (line[j] == '#') {
							grid.emplace_back(i,j);
						}
					}
				}
				presents.push_back({grid});
			}
		}
		inputStream.close();
		//end input


		//so it turns out presents either have 200+ free space or lack a couple of spaces
		int cnt = 0;
		for (auto& treeArea : treeAreas) {
			int occupiedSpace = 0;
			for (int i=0; i<treeArea.presents.size(); i++) {
				occupiedSpace += treeArea.presents.at(i) * presents.at(i).Size();
			}
			int freeSpace = treeArea.w * treeArea.h;
			if (freeSpace < occupiedSpace) {
				std::cout << "no room: lacks " << occupiedSpace - freeSpace << " cells" << "\n";
			}
			else {
				std::cout << "has " << freeSpace - occupiedSpace << " free cells" << "\n";
				cnt++;
			}
		}
		std::cout << cnt << "\n";
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/