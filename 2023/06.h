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

//Races with speeding up boat

struct Task
{
	static inline std::vector<int> times;
	static inline std::vector<int> dists;
	static void run() {

		std::ifstream inputStream("2023/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		std::stringstream ss(line);
		std::string token;
		ss >> token;
		int val = 0;
		while (!ss.eof()) {
			ss >> val;
			times.push_back(val);
		}

		std::getline(inputStream, line);
		ss = std::stringstream(line);
		ss >> token;
		while (!ss.eof()) {
			ss >> val;
			dists.push_back(val);
		}
		inputStream.close();
		int score = 1;
		for (int i=0; i<times.size(); i++) {
			int cnt = 0;
			for (int a = times[i] / 2;; a--) {
				int dist = a * (times[i] - a);
				if (dist > dists[i]) cnt+=2;
				else break;
			}
			if ((times[i] & 1) == 0 && cnt != 0) cnt--;
			score *= cnt;
		}
		std::cout << score << "\n";
	}
	static void runPart2() {
		long long totalTime = 0;
		long long totalDist = 0;
		for (int i=0; i<times.size(); i++) {
			totalTime = totalTime * Utils::powInt(10, Utils::digitsIn(times[i])) + times[i];
			totalDist = totalDist * Utils::powInt(10, Utils::digitsIn(dists[i])) + dists[i];
		}
		double timeD = (double)totalTime;
		double distD = (double)totalDist;
		double min = (timeD - sqrt(timeD * timeD - 4 * distD))/2;
		double max = (timeD + sqrt(timeD * timeD - 4 * distD))/2;
		std::cout << (int)floor(max) - (int)floor(min) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

technically it is just a parabola inequation

x(time - x) > dist
-x^2 + time * x > dist
-x^2 + time * x - c = 0 | c > dist

so intersection points should be at
x = (time +- sqrt(time ^ 2 - 4c))) / 2

*/