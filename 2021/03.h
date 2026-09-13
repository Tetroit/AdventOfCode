#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//calculating most frequent bit in a set

struct Task
{
	static inline std::vector<int> nums;
	static inline int numSize = 0;
	static void run() {

		std::ifstream inputStream("2021/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int total = 0;
		std::vector<int> ones(5);
		while (std::getline(inputStream, line))
		{
			if (numSize == 0) {
				numSize = line.size();
				ones.resize(numSize);
			}
			int num = 0;
			for (int i=0; i<line.size(); i++) {
				int bit = line[i] - '0';
				num = (num << 1) | bit;
				if (bit) ones[i]++;
			}
			nums.push_back(num);
			total++;
		}

		inputStream.close();
		int halfSize = total/2;
		int gamma = 0, epsilon = 0;
		for (int i=0; i<numSize; i++) {
			bool bit = ones[i] >= halfSize;
			gamma = (gamma << 1) | bit;
			epsilon = (epsilon << 1) | !bit;
		}
		std::cout << gamma * epsilon << std::endl;
	}
	static void runPart2() {
		std::vector<int> selection = nums;
		int gamma = 0;
		auto getBit = [](int num, int bit) -> bool {
			int offset = numSize - bit - 1;
			return num >> offset & 1;
		};
		for (int i=0; i<numSize; i++) {
			int oneCnt = 0;
			for (int j = selection.size()-1; j>=0; j--) {
				if (getBit(selection[j], i)) oneCnt++;
			}
			bool bit = oneCnt*2 == selection.size() ? 1 : oneCnt*2 > selection.size();
			gamma = (gamma << 1) | bit;
			for (int j = selection.size()-1; j>=0; j--) {
				if (getBit(selection[j], i) != bit) selection.erase(selection.begin() + j);
			}
		}

		int epsilon = 0;
		selection = nums;
		for (int i=0; i<numSize; i++) {
			int oneCnt = 0;
			if (selection.size() == 1) {
				epsilon = selection[0];
				break;
			}
			for (int j = selection.size()-1; j>=0; j--) {
				if (getBit(selection[j], i)) oneCnt++;
			}
			bool bit = oneCnt*2 == selection.size() ? false : oneCnt*2 < selection.size();
			epsilon = (epsilon << 1) | bit;
			for (int j = selection.size()-1; j>=0; j--) {
				if (getBit(selection[j], i) != bit) selection.erase(selection.begin() + j);
			}
		}

		std::cout << gamma << ' ' << epsilon << ' ' << gamma * epsilon << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/