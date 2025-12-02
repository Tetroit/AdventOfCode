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

//A task with repeating patterns in ids in ranges
struct Task
{
	static bool isValid(long long num) {
		int numDigits = Utils::digitsIn(num);
		if (numDigits % 2 != 0)
			return true;
		long long cut = 1;
		for (int i=0; i<numDigits/2; i++) {
			cut *= 10;
		}
		if (num / cut == num % cut) {
			return false;
		}
		return true;
	}
	static bool isValid2(long long num) {
		int numDigits = Utils::digitsIn(num);

		for (int divisor = 1; divisor < numDigits; divisor++) {
			if (numDigits % divisor != 0)
				continue;
			long long cut = 1;
			int repeats = numDigits/divisor;
			for (int i=0; i<numDigits/repeats; i++) {
				cut *= 10;
			}
			bool isRepeated = true;
			long long copy = num;
			long long etalon = copy % cut;
			copy/=cut;
			for (int i = 0; i < repeats-1; i++) {
				if (etalon != copy % cut) {
					isRepeated = false;
					break;
				}
				copy/=cut;
			}
			if (isRepeated) {
				return false;
			}
		}
		return true;
	}
	static void run() {

	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		long long firstID;
		long long lastID;
		std::vector<std::pair<long long, long long>> ids;
		long long invalidSum = 0;
		while (std::getline(inputStream, line))
		{
			std::vector<std::string> entries = Utils::split(line, ',');
			for (int i = 0; i < entries.size(); i++) {
				char dash;
				std::stringstream ss(entries[i]);
				ss >> firstID;
				ss >> dash;
				ss >> lastID;
				ids.emplace_back(firstID, lastID);
				for (long long j=firstID; j<=lastID; j++) {
					if (!isValid2(j)) {
						invalidSum += j;
					}
				}
			}
		}
		inputStream.close();
		std::cout << invalidSum << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/