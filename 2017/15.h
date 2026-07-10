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
using ull = unsigned long long;
//A and B generating numbers and comparing them
struct Task
{
	static void run() {
		auto AGen = [](ull x) {
			return x * 16807 % 2147483647ull;
		};
		auto BGen = [](ull x) {
			return x * 48271 % 2147483647ull;
		};
		ull A = 512;
		ull B = 191;
		const ull mask = (1<<16) - 1;
		int cnt = 0;
		for (int i=0; i<40'000'000; i++) {
			A = AGen(A);
			B = BGen(B);
			if ((A&mask) == (B&mask))
				cnt++;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		auto AGen = [](ull x) {
			do x = x * 16807 % 2147483647ull;
			while (x & 0b11);
			return x;
		};
		auto BGen = [](ull x) {
			do x = x * 48271 % 2147483647ull;
			while (x & 0b111);
			return x;
		};
		ull A = 512;
		ull B = 191;
		const ull mask = (1<<16) - 1;
		int cnt = 0;
		for (int i=0; i<5'000'000; i++) {
			A = AGen(A);
			B = BGen(B);
			if ((A&mask) == (B&mask))
				cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/