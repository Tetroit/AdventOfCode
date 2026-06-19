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

//calculate triangles
struct Task
{
	static bool checkTri(int val1, int val2, int val3) {
		return (
			val1 + val2 > val3 &&
			val1 + val3 > val2 &&
			val2 + val3 > val1
		);
	}
	static void run() {

		std::ifstream inputStream("2016/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int val1;
		int val2;
		int val3;
		int cnt = 0;
		while (inputStream >> val1 >> val2 >> val3)
		{
			if (val1 + val2 > val3 &&
				val1 + val3 > val2 &&
				val2 + val3 > val1) {
				cnt++;
			}
		}
		std::cout << cnt << std::endl;
		inputStream.close();
	}
	static void runPart2() {
		std::ifstream inputStream("2016/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::array<std::array<int, 3>, 3> batch;
		int cnt = 0;
		while (inputStream
			>> batch[0][0] >> batch[1][0] >> batch[2][0]
			>> batch[0][1] >> batch[1][1] >> batch[2][1]
			>> batch[0][2] >> batch[1][2] >> batch[2][2]
			)
		{
			for (const auto& tri : batch) {
				if (checkTri(tri[0], tri[1], tri[2])) cnt++;
			}
		}
		std::cout << cnt << std::endl;
		inputStream.close();
	}
};

//-------------- NOTES AREA ----------------
/*

*/