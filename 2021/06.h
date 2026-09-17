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
#include <array>
#include "utils.h"

//duplicating lanternfishes
struct Task
{
	static inline std::array<int, 9> fish;
	static void run() {

		std::ifstream inputStream("2021/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int num;
		while (inputStream >> num)
		{
			fish[num] ++;
			inputStream.ignore(1);
		}
		inputStream.close();
		std::array<int, 9> arr = fish;
		constexpr int repeats = 80;
		for (int rep =0; rep<repeats; rep++) {
			int temp = arr[0];
			for (int i=0; i<8; i++) {
				arr[i] = arr[i+1];
			}
			arr[6] += temp;
			arr[8] = temp;
		}
		int cnt = 0;
		for (auto locCnt : arr) {
			cnt+=locCnt;
		}
		std::cout << cnt << std::endl;

	}
	static void runPart2() {
		std::array<long long, 9> arr = {};
		for (int i=0; i<9; i++) {
			arr[i] = fish[i];
		}
		constexpr int repeats = 256;
		for (int rep =0; rep<repeats; rep++) {
			long long temp = arr[0];
			for (int i=0; i<8; i++) {
				arr[i] = arr[i+1];
			}
			arr[6] += temp;
			arr[8] = temp;
		}
		long long cnt = 0;
		for (auto locCnt : arr) {
			cnt+=locCnt;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/