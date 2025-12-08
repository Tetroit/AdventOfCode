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

//The "falling sand task" with timelines and splits
struct Task
{
	static void run() {

		std::ifstream inputStream("2025/input/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::string> map;
		while (std::getline(inputStream, line))
		{
			map.push_back(line);
		}
		inputStream.close();

		int startPos = 0;
		std::vector<int> rows;

		for (int j = 0; j < map[0].size(); j++) {
			if (map[0][j] == 'S') {
				startPos = j;
				rows.push_back(j);
			}
		}
		int splits = 0;
		for (auto & i : map) {
			for (int j = 0; j < rows.size(); j++) {
				int val = rows[j];
				if (i[val] == '^') {
					rows.erase(rows.begin() + j);
					j--;
					splits++;
					int new1 = val - 1;
					int new2 = val + 1;

					if (std::find(rows.begin(), rows.end(), new1) == rows.end()) {
						rows.insert(rows.begin(),new1);
						j++;
					}
					if (std::find(rows.begin(), rows.end(), new2) == rows.end()) {
						rows.insert(rows.begin(),new2);
						j++;
					}
				}
			}
		}
		std::cout << splits << std::endl;
		std::cout << rows.size() << std::endl;

	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::string> map;
		while (std::getline(inputStream, line))
		{
			map.push_back(line);
		}
		inputStream.close();

		int startPos = 0;
		std::vector<int> rows;
		std::unordered_map<int, long long> timelines;

		for (int j = 0; j < map[0].size(); j++) {
			if (map[0][j] == 'S') {
				startPos = j;
				rows.push_back(j);
				timelines.emplace(j, 1);
			}
		}
		int lines = 0;
		for (auto & i : map) {
			for (int j = 0; j < rows.size(); j++) {
				int val = rows[j];
				if (i[val] == '^') {
					rows.erase(rows.begin() + j);
					j--;
					int new1 = val - 1;
					int new2 = val + 1;
					timelines[new1]+=timelines[val];
					timelines[new2]+=timelines[val];
					timelines[val] = 0;
					if (std::find(rows.begin(), rows.end(), new1) == rows.end()) {
						rows.insert(rows.begin(),new1);
						j++;
					}
					if (std::find(rows.begin(), rows.end(), new2) == rows.end()) {
						rows.insert(rows.begin(),new2);
						j++;
					}
				}
			}
		}
		long long totalBranches = 0;
		for (auto&[i, j] : timelines ) {
			totalBranches += j;
		}
		std::cout << rows.size() << std::endl;
		std::cout << totalBranches << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

*/