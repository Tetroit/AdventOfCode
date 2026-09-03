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

//reading values to mem with bitmask
struct Task
{
	static inline std::unordered_map<uint64_t, uint64_t> mem;
	static void run() {

		std::ifstream inputStream("2020/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		uint64_t mask1 = 0;
		uint64_t mask0 = 0;
		while (std::getline(inputStream, line))
		{
			if (line.substr(0, 4) == "mask") {
				mask0 = 0;
				mask1 = 0;
				for (int i = 7; i<line.size(); i++) {
					int offset = 35 - (i - 7);
					if (line[i] == '1')
						mask1 |= (1ULL << offset);
					if (line[i] == '0')
						mask0 |= (1ULL << offset);
				}
			}
			else {
				auto locStr = line.substr(4, line.find_first_of(']') - 4);
				uint64_t loc = std::stoull(locStr);
				int valStart = line.find_last_of(' ') + 1;
				auto valStr = line.substr(valStart, line.size() - valStart);
				uint64_t value = std::stoull(valStr);
				value &= ~mask0;
				value |= mask1;
				mem[loc] = value;
			}
		}
		inputStream.close();
		uint64_t cnt = 0;
		for (auto& [loc, num] : mem) {
			cnt += num;
		}
		std::cout << cnt << std::endl;
		//1535526446 low
		//27064124458542 high
	}
	static void runPart2() {
		mem.clear();
		std::ifstream inputStream("2020/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		uint64_t mask1 = 0;
		std::vector<int> wildBits{};
		while (std::getline(inputStream, line))
		{
			if (line.substr(0, 4) == "mask") {
				wildBits.clear();
				mask1 = 0;
				for (int i = 7; i<line.size(); i++) {
					int offset = 35 - (i - 7);
					if (line[i] == '1')
						mask1 |= (1ULL << offset);
					if (line[i] == 'X')
						wildBits.emplace_back(offset);
				}
			}
			else {
				auto locStr = line.substr(4, line.find_first_of(']') - 4);
				uint64_t loc = std::stoull(locStr);
				int valStart = line.find_last_of(' ') + 1;
				auto valStr = line.substr(valStart, line.size() - valStart);
				uint64_t value = std::stoull(valStr);
				loc |= mask1;
				for (int i=0; i < (1 << wildBits.size()); i++) {
					for (int bit = 0; bit < wildBits.size(); bit++) {
						if (i & (1ULL << bit)) {
							loc |= (1ULL << wildBits[bit]);
						}
						else {
							loc &= ~(1ULL << wildBits[bit]);
						}
					}
					mem[loc] = value;
				}
			}
		}
		inputStream.close();
		uint64_t cnt = 0;
		for (auto& [loc, num] : mem) {
			cnt += num;
		}
		std::cout << cnt << std::endl;
		//1535526446 low
		//27064124458542 high
	}
};

//-------------- NOTES AREA ----------------
/*

*/