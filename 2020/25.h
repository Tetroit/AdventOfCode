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

//decoding encryption keys

struct Task
{
	static void run() {
		int subj = 7;
		int mod = 20201227;
		//the problem is symmetric, so which is which matters not
		int cardKey = 10705932;
		int doorKey = 12301431;
		int cardLoop = 0;
		int doorLoop = 0;
		bool cardLoopFound = false;
		bool doorLoopFound = false;
		{
			int i = 1;
			int loop = 0;
			while (!cardLoopFound && !doorLoopFound) {
				loop++;
				i = i * subj % mod;
				if (i == cardKey) {
					cardLoopFound = true;
					cardLoop = loop;
				}
				if (i == doorKey) {
					doorLoopFound = true;
					doorLoop = loop;
				}
			}
		}
		long long num = 1;
		if (cardLoopFound) {
			for (int i=0; i<cardLoop; i++) {
				num = num * doorKey % mod;
			}
		}
		else if (doorLoopFound) {
			for (int i=0; i<doorLoop; i++) {
				num = num * cardKey % mod;
			}
		}
		std::cout << num << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/