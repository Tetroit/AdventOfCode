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

//redistributing numbers and detecting loops
struct Task
{
	static inline std::vector<std::vector<int>> history;
	static void redistribute(std::vector<int>& banks) {
		int max = 0;
		int maxVal = 0;
		for (int i=0; i<banks.size(); i++) {
			if (banks[i] > maxVal) {
				max = i;
				maxVal = banks[i];
			}
		}
		banks[max] = 0;
		for (int& i : banks) {
			i += maxVal/banks.size();
		}
		int it = max + 1;
		for (int i=0; i<maxVal % banks.size(); i++) {
			if (it == banks.size()) {
				it = 0;
			}
			banks[it++]++;
		}
	}
	static bool alreadyHappened(std::vector<int>& banks) {
		for (const auto& it : history) {
			bool match = true;
			for (int i=0; i<banks.size(); i++) {
				if (banks[i] != it[i]) {
					match = false;
					break;
				}
			}
			if (match)
				return true;
		}
		return false;
	}
	static void run() {

		std::ifstream inputStream("2017/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> banks;
		while (inputStream >> num)
		{
			banks.push_back(num);
		}
		inputStream.close();

		int cnt = 0;
		do {
			history.push_back(banks);
			redistribute(banks);
			cnt++;
		} while (!alreadyHappened(banks));
		history.clear();
		history.push_back(banks);
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		int cnt = 0;
		std::vector<int> banks = history.back();
		history.pop_back();
		do {
			history.push_back(banks);
			redistribute(banks);
			cnt++;
		} while (!alreadyHappened(banks));
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/