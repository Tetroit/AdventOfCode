#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//Reindeer race: run with speed then stop
struct Task
{
	struct DeerStat {
	public:
		int speed;
		int stopTime;
		int flyTime;
		int currentDistance;
		int points;
		DeerStat(int speed, int stopTime, int flyTime) :
		speed(speed),
		stopTime(stopTime),
		flyTime(flyTime),
		points(0),
		currentDistance(0) {}
		void advance(int time) {
			if (time % (flyTime + stopTime) < flyTime) currentDistance += speed;
		}
	};
	static constexpr int seconds = 2503;
	inline static std::vector<DeerStat> deers;

	static void advance(const std::vector<DeerStat>& deers, int time) {

	}
	static void run() {

		std::ifstream inputStream("2015/14.txt");
		std::regex pattern(R"((.+) can fly ([0-9]+) km\/s for ([0-9]+) seconds, but then must rest for ([0-9]+) seconds.)");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::vector<int> distancesAfterN;
		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_search(line, match, pattern);
			std::string name = match[1].str();
			int speed = std::stoi(match[2].str());
			int flyTime = std::stoi(match[3].str());
			int stopTime = std::stoi(match[4].str());
			int period = flyTime + stopTime;
			int cycles = seconds / period;
			int offset = seconds % period;
			int distance = (cycles * flyTime + std::min(flyTime, offset)) * speed;
			distancesAfterN.push_back(distance);
			deers.emplace_back(speed, stopTime, flyTime);
		}
		inputStream.close();

		int max = std::numeric_limits<int>::min();
		for (int& i : distancesAfterN) {
			if (i > max) max = i;
		}
		std::cout << max << "\n";
	}
	static void runPart2() {
		for (int t=0; t<seconds; t++) {
			std::vector<int> lead;
			int leadDist = 0;
			for (int i=0; i < deers.size(); i++) {
				deers[i].advance(t);
				if (deers[i].currentDistance > leadDist) {
					leadDist = deers[i].currentDistance;
					lead.clear();
				}
				if (deers[i].currentDistance >= leadDist) {
					lead.push_back(i);
				}
			}
			for (const int& i : lead) {
				deers[i].points++;
			}
		}
		for (auto & deer : deers) {
			std::cout << "Points: " << deer.points << " Dist: " << deer.currentDistance << "\n";
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/