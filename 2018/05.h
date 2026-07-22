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

//destroying letters with opposite case
struct Task
{
	static std::string collapse(std::string line) {
		int i=0;
		if (line.length() <= 1) {
			std::cout << line.length() << std::endl;
			return line;
		}
		while (i<line.length()) {
			char previous = line[i - 1];
			char current = line[i];
			if (tolower(previous) == tolower(current) && previous != current) {
				line.erase(i-1, 2);
				i--;
				if (i < 1) i = 1;
			}
			else i++;
		}
		return line;
	}
	static std::string remove(std::string line, char c) {
		for (int i=0; i < line.length(); i++) {
			if (tolower(line[i]) == c) {
				line.erase(i, 1);
				i--;
			}
		}
		return line;
	}
	static inline std::string line;
	static void run() {

		std::ifstream inputStream("2018/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::getline(inputStream, line);
		inputStream.close();
		auto res = collapse(line);
		std::cout << res.length() << std::endl;
	}
	static void runPart2() {
		int minLen = std::numeric_limits<int>::max();
		for (char i='a'; i<='z'; i++) {
			int res = collapse(remove(line, i)).length();
			if (res < minLen) minLen = res;
		}
		std::cout << minLen << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/