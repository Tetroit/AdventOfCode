#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Checking for winning numbers

struct Task
{
	static inline std::vector<int> nMatches;
	static void run() {

		std::ifstream inputStream("2023/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int totalScore = 0;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			std::string token;
			int num = 0;
			ss >> token >> num;
			ss.ignore(1);
			std::unordered_set<int> keys;
			while (ss.peek() != '|') {
				ss >> num;
				ss.ignore(1);
				keys.insert(num);
			}
			ss.ignore(2);
			int matches = 0;
			while (!ss.eof()) {
				ss >> num;
				if (keys.contains(num)) matches++;
			}
			int score = 0;
			if (matches > 0) {
				score = 1 << (matches - 1);
			}
			totalScore += score;
			nMatches.push_back(matches);
		}
		inputStream.close();
		std::cout << totalScore << "\n";
	}
	static void runPart2() {
		std::vector<int> dp(nMatches.size(), 1);
		dp[0] = 1;
		for (int i = 0; i < dp.size(); i++) {
			for (int fwd = 1; fwd <= nMatches[i]; fwd++) {
				if (i + fwd >= dp.size()) break;
				dp[i+fwd] += dp[i];
			}
		}
		std::cout << std::accumulate(dp.begin(), dp.end(), 0) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/