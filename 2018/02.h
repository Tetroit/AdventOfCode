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

//checking letter x2 or x3
struct Task
{

	static inline std::vector<std::string> lines;
	static bool similar(const std::string& a, const std::string& b) {
		if (a.size() != b.size()) return false;
		int diff = 0;
		for (int i=0; i < a.length(); i++) {
			if (a[i] != b[i]) {
				diff++;
			}
		}
		return diff == 1;
	}
	static std::string diff(const std::string& a, const std::string& b) {
		if (a.size() != b.size()) return "";
		std::string diffStr;
		for (int i=0; i < a.length(); i++) {
			if (a[i] == b[i])
				diffStr.append(1, a[i]);
		}
		return diffStr;
	}
	static void run() {

		std::ifstream inputStream("2018/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt2 = 0, cnt3 = 0;
		while (std::getline(inputStream, line))
		{
			bool x2 = false, x3 = false;
			std::unordered_map<char, int> freqList;
			for (const auto& ch : line) {
				freqList[ch]++;
			}
			for (const auto& [ch, cnt] : freqList) {
				if (cnt == 2) x2 = true;
				if (cnt == 3) x3 = true;
			}
			if (x2)	cnt2++;
			if (x3)	cnt3++;
			lines.emplace_back(line);
		}
		inputStream.close();
		std::cout << cnt2 * cnt3 << std::endl;
	}
	static void runPart2() {
		for (int i=0; i < lines.size() - 1; i++) {
			for (int j=i+1; j < lines.size(); j++) {
				if (similar(lines[i], lines[j])) {
					std::cout << diff(lines[i], lines[j]) << std::endl;
				}
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/