#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

// spinning and inserting values
struct Task
{
	static void run() {
		const int input = 329;
		const int size = 2017;
		std::vector<int> arr{0};
		int it = 0;
		for (int next=1; next<=size; next++) {
			it += input;
			if (it >= next) {
				it %= next;
			}
			arr.insert(arr.begin() + it++ + 1, next);
		}
		std::cout << arr[it+1] << std::endl;
		std::cout << arr[1] << std::endl;
	}
	static void runPart2() {
		const int input = 329;
		const int size = 50000000;
		std::vector<int> arr{0};
		int it = 0;
		int res = 0;
		for (int next=1; next<=size; next++) {
			it += input;
			if (it >= next) {
				it %= next;
			}
			if (it == 0) {
				res = next;
			}
			it++;
		}
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/