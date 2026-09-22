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

//insertine elements between others in polymers

struct Task
{
	static inline std::unordered_map<std::string, char> inserts;
	static inline std::string initialLine;
	static void run() {

		std::ifstream inputStream("2021/14.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		bool initializedStart = false;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				initializedStart = true;
				continue;
			}
			if (!initializedStart)
				initialLine = line;
			else
				inserts[line.substr(0, 2)] = line[6];
		}
		inputStream.close();

		std::unordered_map<std::string, int> adjacency;
		std::unordered_map<std::string, int> adjacency2;
		for (int i=0; i<initialLine.size()-1; i++) {
			adjacency[initialLine.substr(i, 2)]++;
		}
		auto* src = &adjacency;
		auto* dst = &adjacency2;
		static const int repeats = 10;
		for (int i=0; i<repeats; i++) {
			dst->clear();
			for (auto& [pair, cnt]: *src) {
				char sep = inserts[pair];
				std::string pair1 = {pair[0], sep};
				std::string pair2 = {sep, pair[1]};
				dst->operator[](pair1) += src->operator[](pair);
				dst->operator[](pair2) += src->operator[](pair);
			}
			std::swap(src, dst);
		}
		std::unordered_map<char, int> elements;
		for (auto& [pair, val] : *src) {
			elements[pair[0]]+=val;
			elements[pair[1]]+=val;
		}
		elements[initialLine[0]]++;
		elements[initialLine.back()]++;
		int max = INT_MIN;
		int min = INT_MAX;
		for (auto& [el, val] : elements) {
			val/=2;
			if (val > max) max = val;
			if (val < min) min = val;
		}
		std::cout << max << " " << min << ": " << max - min << std::endl;

	}
	static void runPart2() {
		std::unordered_map<std::string, long long> adjacency;
		std::unordered_map<std::string, long long> adjacency2;
		for (int i=0; i<initialLine.size()-1; i++) {
			adjacency[initialLine.substr(i, 2)]++;
		}
		auto* src = &adjacency;
		auto* dst = &adjacency2;
		static const int repeats2 = 40;
		for (int i=0; i<repeats2; i++) {
			dst->clear();
			for (auto& [pair, cnt]: *src) {
				char sep = inserts[pair];
				std::string pair1 = {pair[0], sep};
				std::string pair2 = {sep, pair[1]};
				dst->operator[](pair1) += src->operator[](pair);
				dst->operator[](pair2) += src->operator[](pair);
			}
			std::swap(src, dst);
		}
		std::unordered_map<char, long long> elements;
		for (auto& [pair, val] : *src) {
			elements[pair[0]]+=val;
			elements[pair[1]]+=val;
		}
		elements[initialLine[0]]++;
		elements[initialLine.back()]++;
		long long max = LLONG_MIN;
		long long min = LLONG_MAX;
		for (auto& [el, val] : elements) {
			val/=2;
			if (val > max) max = val;
			if (val < min) min = val;
		}
		std::cout << max << " " << min << ": " << max - min << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/