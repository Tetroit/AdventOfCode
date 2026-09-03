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

//making adapter chain

struct Task
{
	static inline std::vector<int> adapters;
	static void run() {

		std::ifstream inputStream("2020/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
		{
			adapters.push_back(num);
		}
		inputStream.close();

		std::sort(adapters.begin(), adapters.end());
		int prev = 0;
		int cnt1 = 0, cnt3 = 1;
		for (int curr : adapters) {
			if (curr - prev == 1) cnt1++;
			if (curr - prev == 3) cnt3++;
			prev = curr;
		}
		std::cout << cnt1 * cnt3 << "\n";
	}
	static void runPart2() {
		std::vector<long long> dp(adapters.size());
		for (int i=0; i<3; i++) {
			if (adapters[i] <= 3) dp[i] = 1;
			else break;
		}
		for (int i=0; i<adapters.size(); i++) {
			for (int j=i+1; j<i+4; j++) {
				if (j >= adapters.size()) break;
				if (adapters[i] + 3 >= adapters[j]) dp[j] += dp[i];
				else break;
			}
		}
		std::cout << dp.back() << std::endl;
		//172034048 low
	}
};

//-------------- NOTES AREA ----------------
/*

*/