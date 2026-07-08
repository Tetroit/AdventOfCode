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

//jumping between steps
struct Task
{
	static void run() {

		std::ifstream inputStream("2017/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> jumps;
		while (inputStream >> num)
		{
			jumps.push_back(num);
		}
		inputStream.close();
		int it = 0;
		int cnt = 0;
		while (it < jumps.size() && it >= 0) {
			it += jumps[it]++;
			cnt++;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> jumps;
		while (inputStream >> num)
		{
			jumps.push_back(num);
		}
		inputStream.close();
		int it = 0;
		int cnt = 0;
		while (it < jumps.size() && it >= 0) {
			int& val = jumps[it];
			it += val;
			val += val < 3 ? 1 : -1;
			cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/