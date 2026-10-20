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

//mixing ring of numbers
struct Task
{
	static inline std::vector<int> arr;
	static void run() {

		std::ifstream inputStream("2022/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
		{
			arr.push_back(num);
		}
		inputStream.close();
		std::vector<int> originalToMixed(arr.size());
		std::iota(originalToMixed.begin(), originalToMixed.end(), 0);
		auto mixedToOriginal = originalToMixed;
		for (int i = 0; i < arr.size(); i++) {
			int el = arr[i];
			int mixedPos = originalToMixed[i];
			if (el > 0) {
				for (int j=1; j<=el; j++) {
					int t = Utils::Loop<int>(mixedPos + j, 0, arr.size()-1);
					int& o = originalToMixed[mixedToOriginal[t]];
					o--;
					if (o == -1) o = arr.size() - 1;
					mixedToOriginal[t == 0 ? arr.size() - 1 : t-1] = mixedToOriginal[t];
				}
			}
			else if (el < 0) {
				for (int j=1; j<=-el; j++) {
					int t = Utils::Loop<int>(mixedPos - j, 0, arr.size()-1);
					int& o = originalToMixed[mixedToOriginal[t]];
					o++;
					if (o == arr.size()) o = 0;
					mixedToOriginal[t == arr.size() - 1 ? 0 : t+1] = mixedToOriginal[t];
				}
			}
			int newPos = Utils::Loop<int>(mixedPos + el, 0, arr.size()-1);
			mixedToOriginal[newPos] = i;
			originalToMixed[i] = newPos;
		}
		int zeroPos = 0;
		for (int i = 0; i < arr.size(); i++) {
			if (arr[mixedToOriginal[i]] == 0) zeroPos = i;
		}
		auto valN = [&](int n){return arr[mixedToOriginal[Utils::Loop<int>(zeroPos + n, 0, arr.size()-1)]];};

		std::cout << valN(1000) + valN(2000) + valN(3000) << std::endl;
	}
	static void runPart2() {
		std::vector<long long> arr2;
		for (auto num : arr) {
			arr2.push_back((long long)num * 811589153LL);
		}
		std::vector<int> originalToMixed(arr.size());
		std::iota(originalToMixed.begin(), originalToMixed.end(), 0);
		auto mixedToOriginal = originalToMixed;
		for (int rep = 0; rep < 10; rep++) {
			for (int i = 0; i < arr2.size(); i++) {
				int el = int(arr2[i]%(long long)(arr2.size()-1));
				int mixedPos = originalToMixed[i];
				if (el > 0) {
					for (int j=1; j<=el; j++) {
						int t = Utils::Loop<int>(mixedPos + j, 0, arr2.size()-1);
						int& o = originalToMixed[mixedToOriginal[t]];
						o--;
						if (o == -1) o = arr2.size() - 1;
						mixedToOriginal[t == 0 ? arr2.size() - 1 : t-1] = mixedToOriginal[t];
					}
				}
				else if (el < 0) {
					for (int j=1; j<=-el; j++) {
						int t = Utils::Loop<int>(mixedPos - j, 0, arr2.size()-1);
						int& o = originalToMixed[mixedToOriginal[t]];
						o++;
						if (o == arr2.size()) o = 0;
						mixedToOriginal[t == arr2.size() - 1 ? 0 : t+1] = mixedToOriginal[t];
					}
				}
				int newPos = Utils::Loop<int>(mixedPos + el, 0, arr2.size()-1);
				mixedToOriginal[newPos] = i;
				originalToMixed[i] = newPos;
			}

			for (int i = 0; i < arr2.size(); i++) {
				std::cout << arr2[mixedToOriginal[i]] << ' ';
			}
			std::cout << std::endl;
		}

		int zeroPos = 0;
		for (int i = 0; i < arr2.size(); i++) {
			if (arr2[mixedToOriginal[i]] == 0) zeroPos = i;
		}
		auto valN = [&](int n){return arr2[mixedToOriginal[Utils::Loop<int>(zeroPos + n, 0, arr2.size()-1)]];};

		std::cout << valN(1000) + valN(2000) + valN(3000) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/