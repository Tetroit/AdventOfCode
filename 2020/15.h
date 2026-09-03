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
#include <variant>
#include <vector>
#include "utils.h"

//Game of speaking numbers and how much turns ago the number was said
struct Task
{
	static inline std::vector<int> startSeq;
	static void run() {


		std::ifstream inputStream("2020/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		{
			std::string line;
			int num = 0;
			while (inputStream >> num)
			{
				startSeq.push_back(num);
				inputStream.ignore(1);
			}
			inputStream.close();
		}

		std::unordered_map<int, int> lastTimes;
		for (int i=0; i<startSeq.size()-1; i++) {
			lastTimes[startSeq[i]] = i+1;
		}
		int turn = startSeq.size();
		int lastSpoken = startSeq.back();
		for (;turn != 2020; turn++) {
			int next = lastTimes.contains(lastSpoken) ? turn - lastTimes[lastSpoken] : 0;
			lastTimes[lastSpoken] = turn;
			lastSpoken = next;
		}
		std::cout << lastSpoken << std::endl;
	}
	static void runPart2() {
		constexpr int target = 30000000;
		std::vector<int> lastTimes(target, 0);
		for (int i=0; i<startSeq.size()-1; i++) {
			lastTimes[startSeq[i]] = i+1;
		}
		int lastSpoken = startSeq.back();
		for (int turn = startSeq.size(); turn != target; turn++) {
			int next = lastTimes[lastSpoken] == 0 ? 0 : turn - lastTimes[lastSpoken];
			lastTimes[lastSpoken] = turn;
			lastSpoken = next;
		}
		std::cout << lastSpoken << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/