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

//mapping parameters to other parameters and mapping seed IDs through them

struct Task
{
	struct RangeMap {
		long long dstMin;
		long long srcMin;
		long long size;
		bool inRange(long long val) const {
			return val - srcMin < size && val - srcMin >= 0;
		}
		long long map(long long val) const {
			return val - srcMin + dstMin;
		}
		bool mapRange(std::pair<long long, long long> range,
			std::vector<std::pair<long long, long long>>& rest,
			std::vector<std::pair<long long, long long>>& mapped) const {
			long long mMin = srcMin;
			long long mMax = srcMin + size - 1;
			long long rMin = range.first;
			long long rMax = range.first + range.second - 1;
			if (mMin > rMax || rMin > mMax) return false;
			if (rMin < mMin) rest.emplace_back(rMin, mMin - rMin);
			if (rMax > mMax) rest.emplace_back(mMax + 1, mMax - rMax);
			long long resSize = std::min(mMax, rMax) - std::max(mMin, rMin) + 1;
			mapped.emplace_back(map(std::max(mMin, rMin)), resSize);
			return true;
		}
	};

	static inline std::vector<std::vector<RangeMap>> stages;
	static inline std::vector<long long> seeds;
	static void run() {

		std::ifstream inputStream("2023/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		std::string token;
		long long num = 0;
		std::istringstream ss(line);
		ss >> token;
		while (!ss.eof()) {
			ss >> num;
			seeds.push_back(num);
		}
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				stages.push_back({});
				std::getline(inputStream, line);
				continue;
			}
			ss = std::istringstream(line);
			RangeMap range;
			ss >> range.dstMin;
			ss >> range.srcMin;
			ss >> range.size;
			stages.back().push_back(range);
		}
		inputStream.close();

		long long min = LLONG_MAX;
		for (auto& seed : seeds) {
			long long mapped = seed;
			for (auto& stage : stages) {
				for (auto& range : stage) {
					if (range.inRange(mapped)) {
						mapped = range.map(mapped);
						break;
					}
				}
			}
			min = std::min(min, mapped);
		}
		std::cout << min << std::endl;
	}
	static void runPart2() {
		long long min = LLONG_MAX;
		std::vector<std::pair<long long, long long>> mappedRanges;
		for (int i=0; i<seeds.size(); i+=2) {
			mappedRanges.emplace_back(seeds[i], seeds[i+1]);
		}
		std::vector<std::pair<long long, long long>> nextRanges;
		for (auto& stage : stages) {
			while (!mappedRanges.empty()) {
				auto inputRange = mappedRanges.front();
				mappedRanges.erase(mappedRanges.begin());
				bool found = false;
				for (const auto& range : stage) {
					if (range.mapRange(inputRange, mappedRanges, nextRanges)) {
						found = true;
						break;
					}
				}
				if (!found) {
					nextRanges.push_back(inputRange);
				}
			}
			mappedRanges = nextRanges;
			nextRanges.clear();
		}
		for (auto & mappedRange : mappedRanges) {
			if (mappedRange.first < min) {
				min = mappedRange.first;
			}
		}
		std::cout << min << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/