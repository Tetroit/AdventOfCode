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

//Iterative arithmetic progression
struct Task
{
	static inline std::vector<std::vector<int>> sequences;
	static int next(const std::vector<int>& seq) {
		if (seq.size() == 1) {
			return seq[0];
		}
		std::vector<int> diffs {seq[1] - seq[0]};
		bool sameDiff = true;
		for (int i=2; i < seq.size(); i++) {
			diffs.push_back(seq[i] - seq[i-1]);
			if (diffs.back() != diffs.front()) sameDiff = false;
		}
		if (sameDiff) {
			return seq.back() + diffs.back();
		}
		int nextDiff = next(diffs);
		return nextDiff + seq.back();
	}
	static int prev(const std::vector<int>& seq) {
		if (seq.size() == 1) {
			return seq[0];
		}
		std::vector<int> diffs {seq[1] - seq[0]};
		bool sameDiff = true;
		for (int i=2; i < seq.size(); i++) {
			diffs.push_back(seq[i] - seq[i-1]);
			if (diffs.back() != diffs.front()) sameDiff = false;
		}
		if (sameDiff) {
			return seq.front() - diffs.back();
		}
		int prevDiff = prev(diffs);
		return seq.front() - prevDiff;
	}
	static void run() {

		std::ifstream inputStream("2023/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			sequences.emplace_back();
			int num;
			while (ss >> num) {
				sequences.back().push_back(num);
			}
		}
		inputStream.close();
		int sum = 0;
		for (auto& seq : sequences) {
			sum += next(seq);
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {

		int sum = 0;
		for (auto& seq : sequences) {
			sum += prev(seq);
		}
		std::cout << sum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/