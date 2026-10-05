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

//rock paper scissors

struct Task
{
	static int play(int val1, int val2) {
		if (val1 == val2) return val2 + 3;
		int diff = val2 - val1;
		if (diff == 1 || diff == -2) return val2 + 6;
		return val2;
	}
	static int choose(int val1, int outcome) {
		if (outcome == 2) return val1 + 3;
		if (outcome == 1) return val1 == 1 ? 3 : val1-1;
		return (val1 == 3 ? 1 : val1+1) + 6;
	}
	static void run() {

		std::ifstream inputStream("2022/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char ch1;
		char ch2;
		int score = 0;
		while (inputStream >> ch1 >> ch2)
		{
			int val1 = ch1 - 'A' + 1;
			int val2 = ch2 - 'X' + 1;
			score += play(val1, val2);
		}
		inputStream.close();
		std::cout << score << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2022/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char ch1;
		char ch2;
		int score = 0;
		while (inputStream >> ch1 >> ch2)
		{
			int val1 = ch1 - 'A' + 1;
			int val2 = ch2 - 'X' + 1;
			score += choose(val1, val2);
		}
		inputStream.close();
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/