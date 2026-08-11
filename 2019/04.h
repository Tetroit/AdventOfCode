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

//checking number for conditions
struct Task
{
	static constexpr int start = 246540;
	static constexpr int end = 787419;
	static bool check(int num, bool part2 = false) {
		if (num < 100000 || num > 999999) return false;
		std::vector<int> digits(6);
		for (int d=5; d>=0; d--) {
			digits[d] = num%10;
			num/=10;
		}
		bool doubleDigit = false;
		int sequence = 1;
		for (int i=1; i<6; i++) {
			if (digits[i] < digits[i-1]) return false;
			if (digits[i] == digits[i-1]) {
				if (!part2)
					doubleDigit = true;
				sequence++;
			}
			else {
				if (part2) {
					if (sequence == 2) doubleDigit = true;
				}
				sequence = 1;
			}
		}

		if (part2 && sequence == 2) doubleDigit = true;
		if (!doubleDigit) return false;
		return true;
	}
	static void run() {
		int cnt = 0;
		for (int i = start; i <= end; i++){
			if (check(i)) {
				cnt++;
				std::cout << i << std::endl;
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		int cnt = 0;
		for (int i = start; i <= end; i++){
			if (check(i, true)) {
				cnt++;
				std::cout << i << std::endl;
			}
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/