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

struct Task
{
	static int findLargest(const std::string& line, int startPos, int endPos, int& maxPos) {
		int max = 0;
		for (int i = startPos; i <= endPos; i++ ) {
			int value = line[i] - '0';
			if (value > max) {
				max = value;
				maxPos = i;
			}
		}
		return max;
	}
	static long long findNLargest(const std::string& line, int N) {
		int previousPos = -1;
		long long result = 0;
		for (int i = 0; i < N; i++) {
			int next = findLargest(line, previousPos + 1, line.size() - (N - i), previousPos);
			result = result * 10 + next;
		}
		return result;
	}
	static void run() {

		std::ifstream inputStream("2025/input/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::string> banks;
		while (std::getline(inputStream, line))
		{
			banks.push_back(line);
		}
		inputStream.close();
		int score = 0;
		for (const std::string& bank : banks) {
			int firstMaxPos = 0;
			int firstMax = findLargest(bank, 0, bank.size() - 2, firstMaxPos);
			int secondMaxPos = 0;
			int secondMax = findLargest(bank, firstMaxPos + 1, bank.size() - 1, secondMaxPos);
			int val = firstMax * 10 + secondMax;
			score += val;
		}
		std::cout << score << "\n";
	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::string> banks;
		while (std::getline(inputStream, line))
		{
			banks.push_back(line);
		}
		inputStream.close();
		long long score = 0;
		for (const std::string& bank : banks) {

			long long value = findNLargest(bank, 12);
			score += value;
		}

		std::cout << score << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*

*/