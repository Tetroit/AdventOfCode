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

//getting first and last digits

struct Task
{
	static inline const std::vector<std::string> spells{
		"afkgjskhjn../!",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
	};
	static void run() {

		std::ifstream inputStream("2023/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int sum = 0;
		while (std::getline(inputStream, line))
		{
			int num = 0;
			for (int i=0; i<line.size(); i++) {
				if (std::isdigit(line[i])) {
					num = 10 * (line[i] - '0');
					break;
				}
			}
			for (int i=line.size()-1; i>=0; i--) {
				if (std::isdigit(line[i])) {
					num += line[i] - '0';
					break;
				}
			}
			sum += num;
		}
		inputStream.close();
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2023/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int sum = 0;
		while (std::getline(inputStream, line))
		{
			int num = 0;
			for (int i=0; i<line.size(); i++) {
				bool found = false;
				for (int dig = 0; dig < 10; dig++) {
					if (i + spells[dig].size() > line.size()) continue;
					if (line.substr(i, spells[dig].size()) == spells[dig]) {
						num = 10 * dig;
						found = true;
						break;
					}
				}
				if (found) break;
				if (std::isdigit(line[i])) {
					num = 10 * (line[i] - '0');
					break;
				}
			}
			for (int i=line.size()-1; i>=0; i--) {
				bool found = false;
				for (int dig = 0; dig < 10; dig++) {
					if (i + spells[dig].size() > line.size()) continue;
					if (line.substr(i, spells[dig].size()) == spells[dig]) {
						num += dig;
						found = true;
						break;
					}
				}
				if (found) break;
				if (std::isdigit(line[i])) {
					num += line[i] - '0';
					break;
				}
			}
			sum += num;
		}
		inputStream.close();
		std::cout << sum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/