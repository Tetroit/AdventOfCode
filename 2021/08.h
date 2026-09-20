#pragma once

#include <array>
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

//figuring 7-segment display connection

struct Signals {
	std::array<std::string, 10> start;
	std::array<std::string, 4> output;
};
struct Task
{
	static inline std::vector<Signals> displays;

	static inline const std::array<int, 10> nSegments{
		6,2,5,5,4,5,6,3,7,6
	};
	static inline const std::unordered_map<int, std::vector<int>> digitsBySegments{
		{2, {1}},
		{3, {7}},
		{4, {4}},
		{5, {2,3,5}},
		{6, {6,9,0}},
		{7,{8}}
	};
	static inline const std::array<std::array<uint8_t, 7>, 10> segmentsOfDigit{{
		{1, 1, 0, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 0, 1},
		{1, 0, 1, 1, 1, 1, 0},
		{1, 0, 1, 1, 0, 1, 1},
		{0, 1, 1, 1, 0, 0, 1},
		{1, 1, 1, 0, 0, 1, 1},
		{1, 1, 1, 0, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 0, 1, 1},
	}};
	static void run() {

		std::ifstream inputStream("2021/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			auto words = Utils::split(line, " | ");
			auto startWords = Utils::split(words[0], ' ');
			auto outputWords = Utils::split(words[1], ' ');
			Signals entry;
			for (int i=0; i<10; i++) {
				entry.start[i] = startWords[i];
			}
			for (int i=0; i<4; i++) {
				entry.output[i] = outputWords[i];
			}
			displays.push_back(entry);
		}
		inputStream.close();
		int cnt = 0;
		for (auto& entry : displays) {
			for (int i=0; i<4; i++) {
				auto combo = entry.output[i];
				if (digitsBySegments.at((int)combo.size()).size() == 1) {
					cnt++;
				}
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {

		std::array<std::unordered_map<int, int>, 7> appearencesBySegment{};

		auto matchSegment = [&](std::unordered_map<int, int>& freq) {
			std::vector<int> possible;
			for (int i = 0; i<7; i++) {
				auto& list = appearencesBySegment[i];
				bool match = true;
				for (auto& [n, nums] : digitsBySegments) {
					if (freq[n] != list[n]) match = false;
				}
				if (match) {
					possible.push_back(i);
				}
			}
			return possible;
		};
		for (int i=0; i<10; i++) {
			int n = nSegments[i];
			for (int j=0; j<7; j++) {
				if (segmentsOfDigit.at(i).at(j) == 1) {
					appearencesBySegment[j][n]++;
				}
			}
		}

		int sum = 0;
		for (auto& entry : displays) {
			std::unordered_map<char, std::vector<int>> candidates;
			std::unordered_map<char, std::unordered_map<int, int>> frequencyListPerN;
			for (int i=0; i<10; i++) {
				auto combo = entry.start[i];
				for (char ch : combo) {
					frequencyListPerN[ch][combo.size()]++;
				}
			}
			for (char ch = 'a'; ch < 'a' + 7; ch++) {
				candidates[ch] = matchSegment (frequencyListPerN[ch]);
			}

			for (int i = 0; i<7; i++) {
				for (char ch = 'a'; ch < 'a' + 7; ch++) {
					if (candidates[ch].size() == 1) {
						int match = candidates[ch][0];
						for (auto [candidate, candidateList] : candidates) {
							if (candidate == ch) continue;
							auto it = std::find(candidateList.begin(), candidateList.end(), match);
							if (it != candidateList.end()) {
								candidateList.erase(it);
							}
						}
						break;
					}
				}
			}

			int res = 0;
			for (auto& out : entry.output) {
				int n = out.size();
				std::array<int, 7> mask{0,0,0,0,0,0,0};
				for (char ch : out) {
					mask[candidates[ch][0]] = 1;
				}
				for (auto num : digitsBySegments.at(n)) {
					bool match = true;
					for (int i=0; i<7; i++) {
						if (mask[i] != segmentsOfDigit.at(num).at(i)) {
							match = false;
							break;
						}
					}
					if (match) {
						res = res * 10 + num;
						break;
					}
				}
			}
			sum += res;
		}
		std::cout << sum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

I ID'd it like this
.000.
1   3
1   3
.222.
4   6
4   6
.555.


*/