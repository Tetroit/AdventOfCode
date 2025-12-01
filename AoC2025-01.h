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

//The task to dial a number on a wheel from 0 to 99 and count 0s
struct Task
{
	static void loop(int& num, int min, int max, int& warps) {
		int range = max - min + 1;
		warps = abs(num / range);
		int result = (num - min) % range;
		if (result < 0) {
			result += range;
			warps++;
		}
		num = result + min;
	}
	static void run() {

		std::ifstream inputStream("input2025-01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::regex pattern (R"((L|R)(\d+))");
		std::string line;
		int dial = 50;
		int zeroStops = 0;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, pattern)) {
				int value = std::stoi(match[2]);
				if (match[1] == "L") {
					value = -value;
				}
				dial += value;
				int warps = 0;
				loop(dial, 0, 99, warps);
				if (dial == 0)
					zeroStops++;
			}
		}
		inputStream.close();
		std::cout << zeroStops << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("input2025-01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::regex pattern (R"((L|R)(\d+))");
		std::string line;
		int dial = 50;
		int zeroStops = 0;
		int totalWarps = 0;
		int prevDial = 50;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, pattern)) {
				int value = std::stoi(match[2]);
				if (match[1] == "L") {
					value = -value;
				}
				dial += value;
				int warps = 0;
				loop(dial, 0, 99, warps);

				if (dial == 0)
					zeroStops++;
				if ((dial == 0 && value > 0) || (prevDial == 0 && value < 0))
					warps--;

				totalWarps+=warps;
				prevDial = dial;
			}
		}
		inputStream.close();
		std::cout << totalWarps + zeroStops << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/