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

//counting symbol frequency in each line

struct Task
{
	using FreqList = std::unordered_map<char, int>;
	static inline std::vector<FreqList> stat;
	static void run() {
		std::ifstream inputStream("2016/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		bool lengthSet = false;
		while (std::getline(inputStream, line))
		{
			if (!lengthSet) {
				stat.resize(line.length());
				lengthSet = true;
			}
			for (int i=0; i < line.length(); i++)
				stat[i][line[i]]++;
		}
		inputStream.close();

		std::string res = "";
		for (const FreqList& f : stat) {\
			int max = 0;
			char maxChar = ' ';
			for (auto& [ch, val] : f) {
				if (max < val) {
					max = val;
					maxChar = ch;
				}
			}
			res += maxChar;
		}
		std::cout << res << std::endl;
	}
	static void runPart2() {
		std::string res = "";
		for (const FreqList& f : stat) {\
			int min = std::numeric_limits<int>::max();
			char minChar = ' ';
			for (auto& [ch, val] : f) {
				if (min > val) {
					min = val;
					minChar = ch;
				}
			}
			res += minChar;
		}
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/