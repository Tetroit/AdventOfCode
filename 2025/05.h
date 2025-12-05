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

//Task about fresh ingredients ids and merging ranges
struct Task
{
	static bool overlap(long long lowerA, long long upperA, long long lowerB, long long upperB) {
		return !(lowerA > upperB || upperA < lowerB);
	}
	static bool merge(long long& lowerA, long long& upperA, long long lowerB, long long upperB) {
		if (!overlap(lowerA, upperA, lowerB, upperB))
			return false;
		if (lowerA > lowerB) lowerA = lowerB;
		if (upperA < upperB) upperA = upperB;
		return true;
	}
	static void mergeWithRanges(std::vector<std::pair<long long, long long>>& ranges, long long newLower, long long newUpper) {

		std::vector<int> toRemove;
		for (int i=0; i<ranges.size(); i++) {
			auto&[lower, upper] = ranges[i];
			if (merge(newLower, newUpper, lower, upper)) {
				toRemove.push_back(i);
			}
		}
		ranges.emplace_back(newLower, newUpper);
		for (int i = toRemove.size() - 1; i >= 0; i--) {
			ranges.erase(ranges.begin() + toRemove[i]);
		}
	}
	static void run() {

		std::ifstream inputStream("2025/input/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::pair<long long, long long>> ranges;
		std::vector<long long> values;
		bool inputRanges = true;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				inputRanges = false;
				continue;
			}
			if (inputRanges) {
				int div = line.find('-');
				long long lower = stoll(line.substr(0, div));
				long long upper = stoll(line.substr(div + 1));
				ranges.emplace_back(lower, upper);
			}
			else {
				values.push_back(stoll(line));
			}
		}
		inputStream.close();

		int freshCnt = 0;
		for (auto& val : values) {
			bool isFresh = false;
			for (auto& [lower, upper] : ranges) {
				if (val >= lower && val <= upper) {
					freshCnt++;
					isFresh = true;
					break;
				}
			}
		}
		std::cout << freshCnt << "\n";
	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::pair<long long, long long>> ranges;
		bool inputRanges = true;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				break;
			}
			if (inputRanges) {
				int div = line.find('-');
				long long lower = stoll(line.substr(0, div));
				long long upper = stoll(line.substr(div + 1));
				mergeWithRanges(ranges, lower, upper);
			}
		}
		inputStream.close();

		long long freshCntAll = 0;
		for (auto& [lower, upper] : ranges) {
			freshCntAll += upper - lower + 1;
		}
		std::cout << freshCntAll << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*

*/