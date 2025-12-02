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
		long long cut = Utils::powInt(10, numDigits / 2);
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
			int repeats = numDigits/divisor;
			long long cut = Utils::powInt(10, divisor);
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
	static long long findAddAllDoubles (long long first, long long second, int* numbersConsidered, int* numbersFound) {
		long long sum = 0;
		//first find the first match to a pattern with period of span
		int firstLen = Utils::digitsIn(first);
		int secondLen = Utils::digitsIn(second);

		if (firstLen % 2 != 0) {
			int elongated = firstLen + 1;
			if (secondLen < elongated) {
				return sum;
			}
			firstLen = elongated;
			first = Utils::powInt(10, elongated-1);
		}

		int span = firstLen/2;
		long long mask = Utils::powInt(10, span) + 1;
		if (first % mask != 0) {
			first += mask - first % mask;
		}
		if (second < first) {
			return sum;
		}

		long long nextBreak = Utils::powInt(10, 2 * span) - 1;
		while (first <= second) {
			sum += first;
			if (numbersConsidered != nullptr)
				(*numbersConsidered)++;
			if (numbersFound != nullptr)
				(*numbersFound)++;
			if (first == nextBreak) {
				span++;
				firstLen+=2;
				mask = Utils::powInt(10, span) + 1;
				first = mask * (mask/10);
				nextBreak = Utils::powInt(10, 2 * span) - 1;
			}
			else {
				first += mask;
			}
		}
		return sum;
	}
	static long long findAddAllRepeatsWithSpan (long long first, long long second, int span, int* numbersConsidered, int* numbersFound) {
		long long sum = 0;
		//first find the first match to a pattern with period of span
		long long spanCut = Utils::powInt(10, span);
		int firstLen = Utils::digitsIn(first);
		int secondLen = Utils::digitsIn(second);
		if (firstLen % span != 0 || firstLen == 1) {
			int elongated = firstLen + (span - firstLen % span);
			if (secondLen < elongated) {
				return sum;
			}
			firstLen = elongated;
			first = Utils::powInt(10, elongated-1);
		}
		//adjust to mask
		int repeats = firstLen / span;
		long long mask = 0;
		for (int i=0; i < repeats; i++) {
			mask += Utils::powInt(10, i * span);
		}
		if (first % mask != 0) {
			first += mask - first % mask;
		}
		if (second < first) {
			return sum;
		}
		//loop only masked numbers
		int nextBreak = Utils::powInt(10, repeats * span) - 1;
		while (first <= second) {
			if (numbersConsidered != nullptr)
				(*numbersConsidered)++;
			if (isValid2(first / mask)) {
				sum += first;
				if (numbersFound != nullptr)
					(*numbersFound)++;
			}
			if (first == nextBreak) {
				repeats++;
				mask += Utils::powInt(10, (repeats - 1) * span);
				first = mask * (spanCut/10);
				nextBreak = Utils::powInt(10, repeats * span) - 1;
			}
			else {
				first += mask;
			}
		}
		return sum;
	}
	static long long findAddAllRepeats(long long first, long long second, int* numbersConsidered, int* numbersFound) {
		long long sum = 0;
		for (int i=Utils::digitsIn(second)/2; i>0; i--) {
			sum += findAddAllRepeatsWithSpan(first, second, i, numbersConsidered, numbersFound);
		}
		return sum;
	}
	static void run() {

		std::ifstream inputStream("2025/input/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		long long firstID;
		long long lastID;
		long long invalidSum = 0;
		int numbersConsidered = 0;
		int numbersFound = 0;
		while (std::getline(inputStream, line))
		{
			std::vector<std::string> entries = Utils::split(line, ',');
			for (const auto & entry : entries) {
				char dash;
				std::stringstream ss(entry);
				ss >> firstID;
				ss >> dash;
				ss >> lastID;
				invalidSum += findAddAllDoubles(firstID, lastID, &numbersConsidered, &numbersFound);
				// ids.emplace_back(firstID, lastID);
				// for (long long j=firstID; j<=lastID; j++) {
				// 	if (!isValid(j)) {
				// 		invalidSum += j;
				// 	}
				// }
			}
		}
		inputStream.close();
		std::cout << invalidSum << std::endl;
		std::cout << numbersConsidered << " - " << numbersFound << std::endl;
	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		long long firstID;
		long long lastID;
		long long invalidSum = 0;
		int numbersConsidered = 0;
		int numbersFound = 0;
		while (std::getline(inputStream, line))
		{
			std::vector<std::string> entries = Utils::split(line, ',');
			for (const auto & entry : entries) {
				char dash;
				std::stringstream ss(entry);
				ss >> firstID;
				ss >> dash;
				ss >> lastID;
				invalidSum += findAddAllRepeats(firstID, lastID, &numbersConsidered, &numbersFound);
				// for (long long j=firstID; j<=lastID; j++) {
				// 	if (!isValid2(j)) {
				// 		invalidSum += j;
				// 	}
				// }
			}
		}
		inputStream.close();
		std::cout << invalidSum << std::endl;
		std::cout << numbersConsidered << " - " << numbersFound << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
4174379265 YES
4174379265
4174378255
4174822699
*/