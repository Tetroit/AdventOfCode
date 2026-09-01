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

//Searching 2/3 nums summing to 2020

struct Task
{
	static inline std::vector<int> nums;
	static void run() {

		std::ifstream inputStream("2020/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
			nums.emplace_back(num);
		inputStream.close();
		for (int i = 0; i < nums.size(); ++i) {
			if (nums[i] >= 1010) continue;
			for (int j = 0; j < nums.size(); ++j) {
				if (j==i) continue;
				if (nums[i] + nums[j] == 2020) {
					std::cout << nums[i] * nums[j] << std::endl;
					return;
				}
			}
		}
	}
	static void runPart2() {
		for (int i = 0; i < nums.size(); ++i) {
			if (nums[i] >= 2020/3) continue;
			for (int j = 0; j < nums.size(); ++j) {
				if (j==i) continue;
				for (int k=0; k<nums.size(); ++k) {
					if (j==k) continue;
					if (nums[i] + nums[j] + nums[k] == 2020) {
						std::cout << nums[i] * nums[j] * nums[k] << std::endl;
						return;
					}
				}
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/