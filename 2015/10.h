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
#include "../utils.h"

//It is the size of look-and-say sequence: 1 11 21 1211
struct Task
{
	static std::vector<uint8_t> iterate (const std::vector<uint8_t>& sequence) {
		std::vector<uint8_t> result;
		int i = 0;
		while (i < sequence.size()) {
			int key = sequence[i];
			int count = 0;
			while (i < sequence.size() && sequence[i] == key) {
				i++;
				count++;
			}
			result.push_back(count);
			result.push_back(key);
		}
		return result;
	}
	static void run() {

		std::ifstream inputStream("10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<uint8_t> digits;
		while (std::getline(inputStream, line))
		{
			for (char i : line) {
				digits.push_back((uint8_t) (i - '0'));
			}
		}
		for (int i = 0; i < 40; i++) {
			digits = iterate(digits);
		}
		std::cout << digits.size() << '\n';
		inputStream.close();
	}
	static void runPart2() {
		std::ifstream inputStream("10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<uint8_t> digits;
		while (std::getline(inputStream, line))
		{
			for (char i : line) {
				digits.push_back((uint8_t) (i - '0'));
			}
		}
		for (int i = 0; i < 50; i++) {
			//just runs a bit longer but its fine ig
			digits = iterate(digits);
		}
		std::cout << digits.size() << '\n';
		inputStream.close();
	}
};

//-------------- NOTES AREA ----------------
/*

*/