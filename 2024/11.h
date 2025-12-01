#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "../utils.h"


struct Task
{
	std::pair<uint64_t, uint64_t> split(const uint64_t& num, int at) {
		uint64_t fac = 1;
		for (int i=0; i < at; i++) {
			fac *= 10;
		}
		return {num/fac, num%fac};
	}

	uint64_t countStones(const std::vector<std::pair<uint64_t, uint64_t>>& arr) {
		uint64_t res = 0;
		for (auto& pair : arr) {
			res += pair.second;
		}
		return res;
	}

	void batch (std::vector<std::pair<uint64_t, uint64_t>>& arr) {
		for (int i=0; i < arr.size(); i++) {
			auto& pair = arr[i];
			uint64_t key = pair.first;

			auto j = arr.begin() + i + 1;
			while (j < arr.end()) {
				if (j->first == key) {
					pair.second += j->second;
					j = arr.erase(j);
				}
				else {
					++j;
				}
			}
		}
	}
	void blink (std::vector<std::pair<uint64_t, uint64_t>>& arr) {
		std::vector<std::pair<uint64_t, uint64_t>> newEls;

		auto i = arr.begin();
		while (i < arr.end()) {
			auto& pair = *i;
			auto& num = pair.first;
			// set 1
			if (num == 0) {
				num = 1;
				i++;
				continue;
			}

			//split
			int digits = digitsIn(num);
			if (digitsIn(num) % 2 == 0) {
				auto splitRes = split(num, digits/2);
				newEls.emplace_back(splitRes.first, pair.second);
				newEls.emplace_back(splitRes.second, pair.second);
				i = arr.erase(i);
				continue;
			}

			//multiply
			i++;
			num *= 2024;
		}

		batch(newEls);
		for (auto& pair : newEls) {
			arr.emplace_back(pair);
		}

	}


	//0 -> 1 -> 2024 -> split -> split
	//1 -> 2024 -> split -> split
	//2 -> 2 * 2024 -> split -> split
	//3 -> 3 * 2024 -> split -> split
	//4 -> 4 * 2024 -> split -> split
	//5 -> 5 * 2024 -> 5 * 2024 * 2024 -> split -> split -> split
	//6 -> 6 * 2024 -> 6 * 2024 * 2024 -> split -> split -> split
	//7 -> 7 * 2024 -> 7 * 2024 * 2024 -> split -> split -> split
	//8 -> 8 * 2024 -> 8 * 2024 * 2024 -> split -> split -> split
	//9 -> 9 * 2024 -> 9 * 2024 * 2024 -> split -> split -> split
	// there are 50 ish states if we start from single digits, so it is possible to group them

	const int STEPS = 75;
	void run(){

		for (int i=0; i<std::numeric_limits<int>::max(); i++) {

		}
		std::ifstream inputStream("input2024-11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		std::vector<std::pair<uint64_t, uint64_t>> arr;
		uint64_t num;
		while (inputStream >> num)
		{
			arr.emplace_back(num, 1);
		}
		for (int i=0; i < STEPS; i++) {
			std::cout << "Steps: " << i + 1 << '\n';
			blink(arr);
			std::cout << countStones(arr) << '\n';
			std::cout << arr.size() << '\n';
			std::cout << '\n';
		}
		std::cout << countStones(arr) << '\n';
		std::cout << arr.size() << '\n';
	}
};
