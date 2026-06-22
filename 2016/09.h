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

//repeating sequences
struct Task
{
	static inline std::vector<std::string> transformed = {};
	static std::string decompress(const std::string& line, bool& decompressed) {
		decompressed = true;
		char ch, shit;
		int amount = 0, repeats = 0;
		std::string repeatBuf;
		std::stringstream ss(line);
		std::string res;
		while (ss >> ch)
		{
			if (amount == 0 && ch == '(') {
				ss >> amount >> shit >> repeats >> shit;
				decompressed = false;
				continue;
			}
			res += ch;
			if (amount != 0) {
				repeatBuf += ch;
				amount--;
			}
			if (amount == 0 && !repeatBuf.empty()) {
				for (int i=0; i<repeats - 1; i++) {
					res.insert(res.end(), repeatBuf.begin(), repeatBuf.end());
				}
				repeatBuf.clear();
			}
		}
		return res;
	}
	static void run() {

		std::ifstream inputStream("2016/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			bool decompressed = false;
			transformed.emplace_back(decompress(line, decompressed));
		}
		inputStream.close();
		int cnt = 0;
		for (const auto & i : transformed) {
			cnt+=i.size();
		}
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
	}
};

//-------------- NOTES AREA ----------------
/*

*/