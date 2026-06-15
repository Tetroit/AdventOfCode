#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

// Elves delivering presents
struct Task
{
	static constexpr int total = 36000000;
	static void run() {

		int currentSum = 0;
		int house = 1;
		for (int i=0; i * (i+1) / 2 * 10 < total; i++) {
			house = i;
		}
		while (currentSum < total) {
			house++;
			currentSum = 0;
			for (int i=1; i*i<=house; i++) {
				if (house % i == 0) {
					currentSum += i;
					currentSum += house / i;
				}
				if (i*i == house) {
					currentSum -= i;
				}
			}
			currentSum *= 10;
		}
		std::cout << "House: " << house << std::endl;
	}
	static void runPart2() {
		int currentSum = 0;
		int house = 1;
		for (int i=0; i * (i+1) / 2 * 11 < total; i++) {
			house = i;
		}
		while (currentSum < total) {
			house++;
			currentSum = 0;
			for (int i=1; i<50 && i<house; i++) {
				if (house % i == 0) {
					currentSum += house / i;
				}
				if (i*i == house) {
					currentSum -= i;
				}
			}
			currentSum *= 11;
		}
		std::cout << "House: " << house << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/