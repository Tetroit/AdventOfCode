#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//putting eggnog in containers
struct Task
{
	inline static std::vector<int> buckets;
	inline static int minCount = 0;
	static constexpr int total = 150;
	static std::tuple<int, int> match(const std::vector<int>& list, const int sum) {
		int cnt = 0;

		std::vector<uint8_t> mask;
		std::vector<int> variantCnt;
		variantCnt.resize(list.size(), 0);
		mask.resize(list.size(), 0);

		matchStep(list, sum, mask, variantCnt, 0, 0, 0, cnt);

		int minBuckets = 0;
		for (int i=0; i<list.size(); i++) {
			if (variantCnt[i] != 0) {
				minBuckets = variantCnt[i];
				break;
			}
		}
		return {cnt, minBuckets};
	}
	static void matchStep(const std::vector<int>& list, const int sum, std::vector<uint8_t>& mask, std::vector<int>& varCnt, int cursor, int used, int acc, int& cnt) {

		if (acc == sum) {
			std::cout << "Match found with: ";
			for (int i=0; i<list.size(); i++) {
				if (mask[i] == 1)
					std::cout << list[i] << " ";
			}
			std::cout << std::endl;
			varCnt[used]++;
			cnt++;
			return;
		}
		if (acc > sum) {
			return;
		}
		if (cursor == list.size() - 1) {
			if (acc + list[cursor] == sum) {
				used++;
				std::cout << "Match found with: ";
				for (int i=0; i<list.size(); i++) {
					if (mask[i] == 1 || i == cursor)
						std::cout << list[i] << " ";
				}
				varCnt[used]++;
				std::cout << std::endl;
				cnt++;
			}
			return;
		}
		matchStep(list, sum, mask, varCnt, cursor+1, used, acc, cnt);
		mask[cursor] = 1;
		matchStep(list, sum, mask, varCnt, cursor+1, used+1, acc+list[cursor], cnt);
		mask[cursor] = 0;
	}
	static void run() {

		std::ifstream inputStream("2015/17.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			buckets.push_back(std::stoi(line));
		}
		inputStream.close();
		int cnt = 0;
		std::tie(cnt, minCount) = match(buckets, total);
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::cout << minCount << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/