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

//Guards falling asleep
struct Task
{
	static inline const std::regex shiftPattern {R"(Guard #(\d+) begins shift)", std::regex::optimize};
	static inline std::map<uint64_t, int> events;
	static inline std::unordered_map<int, std::vector<std::pair<uint16_t, uint16_t>>> sleepTimes;
	static void run() {

		std::ifstream inputStream("2018/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::string timestamp = line.substr(0, line.find(']')+1);
			line.erase(0, timestamp.length() + 1);
			uint64_t year, month, day, hour, minute;
			std::istringstream ss(timestamp);
			char shit;
			ss >> shit >> year >> shit >> month >> shit >> day >> hour >> shit >> minute;
			uint64_t timecode = minute + (hour << 8) + (day << 16) + (month << 24) + (year << 32);

			int event = 0;
			if (line == "falls asleep") {
				event = -1;
			}
			else if (line == "wakes up") {
				event = -2;
			}
			else {
				auto begin = line.find('#') + 1;
				event = std::stoi(line.substr(begin, line.find(' ', begin) - begin));
			}
			events.emplace(timecode, event);
		}
		inputStream.close();
		int guardID;
		uint16_t startMinute;
		for (auto& [timecode, event] : events) {
			if (event >= 0) {
				guardID = event;
				continue;
			}
			uint16_t minute = timecode & 63;
			if (event == -1) {
				startMinute = minute;
			}
			if (event == -2) {
				sleepTimes[guardID].emplace_back(startMinute, minute);
			}
		}
		int mostSleep = 0;
		int mostSleepID = -1;
		for (auto& [id, sleeps] : sleepTimes) {
			int totalSleepTime = 0;
			for (const auto& [start, end] : sleeps) {
				totalSleepTime += end - start;
			}
			if (totalSleepTime > mostSleep) {
				mostSleepID = id;
				mostSleep = totalSleepTime;
			}
		}
		std::unordered_map<uint16_t, int> freqList;
		for (uint16_t i=0; i<60; i++) {
			for (const auto& [start, end] : sleepTimes[mostSleepID]) {
				if (i >= start && i < end) {
					freqList[i]++;
				}
			}
		}
		int maxFrequency = 0;
		int maxMinute = -1;
		for (const auto& [minute, freq] : freqList) {
			if (maxFrequency < freq) {
				maxFrequency = freq;
				maxMinute = minute;
			}
		}
		std::cout << maxMinute * mostSleepID << std::endl;
	}
	static void runPart2() {
		std::unordered_map<uint16_t, int> sleepByMinute;
		std::unordered_map<uint16_t, std::unordered_map<int, int>> guardFreqByMinute;
		for (const auto& [guard, sleep] : sleepTimes) {
			for (const auto& [start, end] : sleep) {
				for (int i=start; i<end; i++) {
					sleepByMinute[i]++;
					guardFreqByMinute[i][guard]++;
				}
			}
		}
		int maxGuard = -1;
		int maxGuardFreq = 0;
		int maxMinute = -1;
		for (int i=0; i<60; i++) {
			for (const auto& [guard, cnt] : guardFreqByMinute[i]) {
				if (cnt > maxGuardFreq) {
					maxGuardFreq = cnt;
					maxGuard = guard;
					maxMinute = i;
				}
			}
		}
		std::cout << maxGuard * maxMinute << std::endl;
		//34956
	}
};

//-------------- NOTES AREA ----------------
/*

*/