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

//did number increase or decrease

struct Task
{
	static inline std::vector<int> nums;
	static void run() {

		std::ifstream inputStream("2021/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num, prev;
		inputStream >> prev;
		nums.push_back(prev);
		int cnt = 0;
		while (inputStream >> num)
		{
			if (num > prev) cnt++;
			prev = num;
			nums.push_back(num);
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		int cnt = 0;
		for (int i=3; i<nums.size(); ++i) {
			if (nums[i] > nums[i-3]) cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/