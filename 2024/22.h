#pragma once

#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <thread>
#include <unordered_map>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"

struct Task
{

	int mix (int a, int b) {
		return a ^ b;
	}
	int prune (int a) {
		return a & 0xFFFFFF;
	}
	int next (int a) {
		a = prune(mix(a, a << 6));
		//std::cout << std::bitset<24>(a) << "\n";
		a = prune(mix(a, a >> 5));
		//std::cout << std::bitset<24>(a) << "\n";
		a = prune(mix(a, a << 11));
		//std::cout << std::bitset<24>(a) << "\n";
		return a;
	}

	std::vector<std::vector<int>> cache;
	std::unordered_map<uint32_t, int> stonks;

	void addStonk(uint32_t code, int res) {
		stonks[code] += res;
	}
	void addStonk(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4, int res) {
		addStonk(x1 + (x2 << 8) + (x3 << 16) + (x4 << 24), res);
	}
	int getStonk(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4) {
		int code = x1 + (x2 << 8) + (x3 << 16) + (x4 << 24);
		return stonks[code];
	}

	int getNth(int target, int n) {
		if (n == 0)
			return target;
		int res = 0;
		for (int j =0; j < 24; j++) {
			int bit = (1 << j);
			if ((target & bit) != 0)
				res ^= cache.at(n-1).at(j);
		}
		return res;
	}
	void run() {

		std::ifstream inputStream("input2024-22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<int> arr;
		int num;
		while (inputStream >> num)
		{
			arr.push_back(num);
		}

		//cache all bits and then combine numbers from it
		int cacheIt = 2000;
		int bit = 1;

		std::vector<int> samples;

		for (int i=0; i < 24; i++) {
			samples.push_back(bit);
			bit <<= 1;
		}

		for (int i=0; i<2000; i++) {
			cache.emplace_back(24);
			for (int j =0; j<24; j++) {
				samples[j] = next(samples[j]);
				cache[i][j] = samples[j];
			}
		}

		long long score = 0;

		for (int i=0; i < arr.size(); i++) {
			score += getNth(arr[i], 2000);
		}

		std::cout << score << "\n";


		//part 2




		for (int i=0; i<arr.size(); i++) {

			std::vector<uint32_t> used;
			std::vector<uint8_t> localStonks;

			int value = arr[i];
			auto start0 = (int8_t) (getNth(value, 0) % 10);
			auto start1 = (int8_t) (getNth(value, 1) % 10);
			auto start2 = (int8_t) (getNth(value, 2) % 10);
			auto prev = (int8_t) (getNth(value, 3) % 10);

			int8_t current = 0;

			localStonks.insert(localStonks.begin(),(uint8_t)(start1 - start0));
			localStonks.insert(localStonks.begin(),(uint8_t)(start2 - start1));
			localStonks.insert(localStonks.begin(),(uint8_t)(prev - start2));

			for (int it=4; it<2000; it++) {
				current = (int8_t)(getNth(value, it) % 10);
				localStonks.insert(localStonks.begin(),(uint8_t)(current - prev));

				uint32_t code = localStonks[0] + (localStonks[1] << 8) + (localStonks[2] << 16) + (localStonks[3] << 24);
				if (std::find (used.begin(), used.end(), code) == used.end()) {
					used.push_back(code);
					addStonk(code, current);
				}

				localStonks.pop_back();
				prev = current;
			}
		}
		int max = 0;
		for (auto& [key, bananas] : stonks) {
			if (bananas > max) {
				max = bananas;
			}
		}
		std::cout << max << "\n";

		// int test1 = 5527815;
		// int test1 = 2;
		//
		// int test2 = 21357;
		// int test3 = test1 ^ test2;
		// int start = test1;
		// test1 = next(test1);
		// std::cout << std::bitset<24>(test1) << "\n";
		// int cnt = 0;
		// while ( (start << 1) != (test1)) {
		// 	test1 = next(test1);
		// 	cnt++;
		// 	//std::cout << test1 << "\n";
		// }
		// std::cout << cnt << "\n";

		//1 => 000000100001100001000011
		//2 => 000001000011000010000110
		//3 => 000001100010100011000101

		//           010101000101100100000111
		//     010101000101100100000111
		//           010000100001100011000111
		//                010000100001100011000111
		//           010000000000100000000001
		//010000000000100000000001
		//           000000000000000000000001

		//
		//if a1 ^ b1 = c1
		//then for any n an ^ bn = cn
	}

};
