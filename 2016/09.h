#pragma once

#include <assert.h>
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

//repeating sequences
struct Task
{
	// static inline std::vector<std::string> transformed = {};
	// static std::string decompress(const std::string& line, bool& decompressed) {
	// 	decompressed = true;
	// 	char ch, shit;
	// 	int amount = 0, repeats = 0;
	// 	std::string repeatBuf;
	// 	std::stringstream ss(line);
	// 	std::string res;
	// 	while (ss >> ch)
	// 	{
	// 		if (amount == 0 && ch == '(') {
	// 			ss >> amount >> shit >> repeats >> shit;
	// 			decompressed = false;
	// 			continue;
	// 		}
	// 		res += ch;
	// 		if (amount != 0) {
	// 			repeatBuf += ch;
	// 			amount--;
	// 		}
	// 		if (amount == 0 && !repeatBuf.empty()) {
	// 			for (int i=0; i<repeats - 1; i++) {
	// 				res.insert(res.end(), repeatBuf.begin(), repeatBuf.end());
	// 			}
	// 			repeatBuf.clear();
	// 		}
	// 	}
	// 	return res;
	// }
	static std::tuple<size_t, int, int> capture(const std::string& string, size_t startID) {
		int size, repeats;
		assert(string[startID] == '(');
		size_t subS = startID+1;
		size_t subE = string.find('x', subS);
		size = std::stoi(string.substr(subS,subE - subS));
		subS = subE + 1;
		subE = string.find(')', subS);
		repeats = std::stoi(string.substr(subS,subE - subS));

		return {subE + 1, size, repeats};
	}
	static size_t countDecompress(const std::string& line, size_t start, size_t end, bool recursive = false) {
		size_t amount, repeats;
		size_t cnt = 0;
		for (size_t i=start; i<end;) {
			if (line[i] == '(') {
				std::tie(i, amount, repeats) = capture(line, i);
				if (recursive) {
					cnt += repeats * countDecompress(line, i, i + amount, recursive);
					i+=amount;
				}
				else {
					cnt+=amount*repeats;
					i+=amount;
				}
			}
			else {
				cnt++;
				i++;
			}
		}
		return cnt;
	}
	static size_t countDecompress(const std::string& line, bool recursive = false) {
		return countDecompress(line, 0, line.size(), recursive);
	}
	static void run() {

		std::ifstream inputStream("2016/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		inputStream.close();

		size_t cnt = countDecompress(line);
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		//bruteforce works here lmao, took me 511 seconds tho

		// for (auto& line : transformed) {
		// 	bool decompressed = false;
		// 	while (!decompressed) {
		// 		line = decompress(line, decompressed);
		// 		std::cout << line.length() << std::endl;
		// 	}
		// }
		// int cnt = 0;
		// for (const auto & i : transformed) {
		// 	cnt+=i.size();
		// }
		// std::cout << cnt << std::endl;


		//the canon boring solution
		std::ifstream inputStream("2016/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		inputStream.close();

		size_t cnt = countDecompress(line, true);
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
*/