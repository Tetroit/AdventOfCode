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

//cellular automata with 5 neighbours

struct Task
{
	static constexpr size_t bit_size = 1024;
	static inline std::unordered_map<uint8_t, bool> patterns;

	static std::bitset<bit_size> next(const std::bitset<bit_size>& from, int& start, int& end) {
		std::bitset<bit_size> res;
		bool firstFound = false;
		int first = 0;
		int last = 0;
		for (int i=start - 2; i<end+2; i++) {
			uint8_t key = 0;
			for (int bit =0; bit < 5; bit++) {
				key |= (from[i+bit-2] << bit);
			}
			bool val = patterns[key];
			if (val) {
				if (!firstFound) {
					firstFound = true;
					first = i;
				}
				last = i + 1;
			}
			res[i] = val;
		}
		start = first;
		end = last;
		return res;
	}
	static void run() {

		std::ifstream inputStream("2018/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line, in;
		char out;
		std::bitset<bit_size> bits;
		inputStream >> line >> line >> line;
		int start = bit_size/2;
		int end = start + line.length();
		for (int i=start; i<end; i++) {
			bits[i] = line[i - start] == '#' ? true : false;
		}
		std::getline(inputStream, line);
		while (inputStream >> in >> line >> out)
		{
			uint8_t pattern = 0;
			for (int i=0; i<5; i++) {
				if (in[i] == '#') pattern |= (1 << i);
			}
			patterns.emplace(pattern, out == '#');
		}
		inputStream.close();
		for (int i=0;i<20;i++) {
			bits = next(bits, start, end);
		}
		int cnt = 0;
		for (int i=start; i<end; i++) {
			if (bits[i])
				cnt += i - bit_size/2;
		}
		std::cout << std::endl;
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2018/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line, in;
		char out;
		std::bitset<bit_size> bits;
		inputStream >> line >> line >> line;
		int start = bit_size/2;
		int end = start + line.length();
		for (int i=start; i<end; i++) {
			bits[i] = line[i - start] == '#' ? true : false;
		}
		std::getline(inputStream, line);
		while (inputStream >> in >> line >> out)
		{
			uint8_t pattern = 0;
			for (int i=0; i<5; i++) {
				if (in[i] == '#') pattern |= (1 << i);
			}
			patterns.emplace(pattern, out == '#');
		}
		inputStream.close();
		long long iterations = 50000000000;
		for (int i=0;i<100;i++) {
			bits = next(bits, start, end);
			iterations--;
			for (int i=bit_size/2 - 10; i<bit_size/2+200; i++) {
				std::cout << (bits[i] ? '#' : '.');
			}
			std::cout << std::endl;
		}
		long long score = 0;
		long long pots = 0;
		for (int i=start; i<end; i++) {
			if (bits[i]) {
				score += i - bit_size/2;
				pots++;
			}
		}
		score += iterations * pots;
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

after a 100 iterations the patters seems to just be shifting 1 position to the right,
so each pot "moves" to the right once per iteration increasing score by 1.

*/