#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cmath>
#include <functional>
#include <list>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//making new numbers from digits of sum of elements
struct Task
{
	template <typename T>
	static void loopPrev(const std::list<T>& list, std::list<T>::const_iterator& it, int n) {
		for (int i = 0; i < n; ++i) {
			if (it == list.begin()) it = list.end();
			--it;
		}
	}
	template <typename T>
	static void loopNext(const std::list<T>& list, std::list<T>::const_iterator& it, int n) {
		for (int i = 0; i < n; ++i) {
			++it;
			if (it == list.end()) it = list.begin();
		}
	}
	static constexpr int goal = 170641;
	static constexpr int pow10(int pow) {
		int val = 1;
		for (int i=0; i<pow; ++i) {
			val *= 10;
		}
		return val;
	}
	static constexpr std::vector<uint8_t> asDigits(int num) {
		if (num == 0) return {0};
		std::vector<uint8_t> digits;
		for (int i=Utils::digitsIn(num) - 1; i>=0; --i) {
			digits.push_back(num / pow10(i) % 10);
		}
		return digits;
	}
	static void run() {

		std::list<uint8_t> recipes {3,7};
		auto ptr1 = recipes.cbegin();
		auto ptr2 = recipes.cbegin();
		ptr2++;
		while (recipes.size() < goal + 10) {
			uint8_t sum = *ptr1 + *ptr2;
			int digits = Utils::digitsIn(sum);
			for (int i=0; i<digits; ++i) {
				recipes.push_back(sum / pow10(digits - i - 1) % 10);
			}
			loopNext(recipes, ptr1, 1 + *ptr1);
			loopNext(recipes, ptr2, 1 + *ptr2);
		}
		auto reader = recipes.cbegin();
		loopNext(recipes, reader, goal);
		for (int i=0; i<10; ++i) {
			std::cout << (int) *reader;
			++reader;
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		auto seq = asDigits(goal);
		// seq = {1,2,4,5};
		std::list<uint8_t> recipes = {3,7};
		auto ptr1 = recipes.cbegin();
		auto ptr2 = recipes.cbegin();
		ptr2++;
		auto reader = recipes.cbegin();
		int overlap = 0;
		long long readerPos = 0;
		while (true) {
			uint8_t sum = *ptr1 + *ptr2;
			int digits = Utils::digitsIn(sum);
			for (int i=0; i<digits; ++i) {
				recipes.push_back(sum / pow10(digits - i - 1) % 10);
			}
			loopNext(recipes, ptr1, 1 + *ptr1);
			loopNext(recipes, ptr2, 1 + *ptr2);
			bool found = false;
			reader++;
			readerPos++;
			while (reader != recipes.cend()) {
				uint8_t val = *reader;
				if (val == seq[overlap]) {
					overlap++;
				}
				else if (overlap > 0) {
					reader = std::prev(reader, overlap-1);
					readerPos-=overlap-1;
					overlap = 0;
					continue;
				}
				if (overlap == seq.size()) {
					found = true;
					break;
				}
				reader++;
				readerPos++;
			}
			if (found) {
				std::cout << readerPos - overlap + 1 << std::endl;
				break;
			}
			reader--;
			readerPos--;
		}
		//23449775
	}
};

//-------------- NOTES AREA ----------------
/*

*/