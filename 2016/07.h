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

// finding abba sequence in strings
struct Task
{
	static bool hasTLS(const std::string& line) {

		int bracket = 0;
		bool hasOutside = false;
		for (int i=3; i<line.length(); i++) {
			if (line[i] == '[') {
				bracket++;
				continue;
			}
			if (line[i] == ']') {
				bracket--;
				continue;
			}
			if (line[i-3] == line[i] && line[i-2] == line[i-1] && line[i-1] != line[i]) {
				if (bracket != 0) {
					return false;
				}
				hasOutside = true;
			}
		}
		return hasOutside;
	}
	static bool hasSSL(const std::string& line) {

		int bracket = 0;
		std::vector<std::string> aba;
		std::vector<std::string> bab;
		for (int i=2; i<line.length(); i++) {
			if (line[i] == '[') {
				bracket++;
				continue;
			}
			if (line[i] == ']') {
				bracket--;
				continue;
			}
			auto isBracket = [](const char& ch) {
				return ch == '[' || ch == ']';
			};
			if (line[i-2] == line[i] && line[i-1] != line[i] && !isBracket(line[i]) && !isBracket(line[i-1])) {
				std::string match = {line[i-1], line[i], line[i-1]};
				if (bracket != 0) {
					if (std::ranges::find(aba, match) != aba.end()) {
						return true;
					}
					bab.push_back(line.substr(i-2, 3));
				}
				else {
					if (std::ranges::find(bab, match) != bab.end()) {
						return true;
					}
					aba.push_back(line.substr(i-2, 3));
				}
			}
		}
		return false;
	}
	static void run() {

		std::ifstream inputStream("2016/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt = 0;
		while (std::getline(inputStream, line))
		{
			if (hasTLS(line)) {
				cnt++;
			}
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2016/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt = 0;
		while (std::getline(inputStream, line))
		{
			if (hasSSL(line)) {
				cnt++;
			}
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/