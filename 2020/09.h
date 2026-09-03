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

//checking if next number is sum of previous 2
struct Task
{
	static constexpr int preamble = 25;
	static inline std::vector<long long> nums;
	static inline long long target;
	static void run() {

		std::ifstream inputStream("2020/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		long long num;
		while (inputStream >> num) {
			nums.push_back(num);
		}
		inputStream.close();

		for (int i=preamble; i<nums.size(); ++i) {
			bool valid = false;
			for (int a = i - preamble; a < i-1; ++a ) {
				for (int b = a; b < i; ++b ) {
					if (nums[a] + nums[b] == nums[i]) {
						valid = true;
					}
				}
			}
			if (!valid) {
				std::cout << nums[i] << std::endl;
				target = nums[i];
				break;
			}
		}
	}
	static void runPart2() {
		for (int start = 0; start < nums.size()-1; ++start) {
			long long sum = nums[start];
			for (int end = start+1; end < nums.size(); ++end) {
				sum+=nums[end];
				if (sum > target) break;
				if (sum == target) {
					long long min = LLONG_MAX;
					long long max = LLONG_MIN;
					for (int i=start; i<=end; ++i) {
						if (nums[i] < min) min = nums[i];
						if (nums[i] > max) max = nums[i];
					}
					std::cout << min + max << std::endl;
					return;
				}
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/