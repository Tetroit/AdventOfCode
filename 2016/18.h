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

//making rows from other rows and counting safe cells

struct Task
{
	static void nextRow(const std::vector<uint8_t>& row1, std::vector<uint8_t>& row2) {
		row2.resize(row1.size());
		row2[0] = (row1[1] == 1);
		for (int i=1; i<row1.size()-1; i++) {
			row2[i] = (row1[i-1] != row1[i+1]);
		}
		row2[row1.size()-1] = (row1[row1.size()-2] == 1);
	}
	static int countSafe(const std::vector<uint8_t>& row) {
		int cnt = 0;
		for (unsigned char i : row) {
			if (i == 0) {
				cnt++;
			}
		}
		return cnt;
	}
	static void printRow(const std::vector<uint8_t>& row) {
		for (auto i : row) {
			std::cout << (i == 1 ? '^' : '.');
		}
		std::cout << std::endl;
	}
	static constexpr int steps = 40;
	static constexpr int steps2 = 400000;
	static void run() {

		std::ifstream inputStream("2016/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char ch;
		std::vector<uint8_t> line1;
		std::vector<uint8_t> line2;
		while (inputStream >> ch)
		{
			line1.push_back(ch == '^');
		}
		inputStream.close();
		bool back = false;
		int cnt = countSafe(line1);
		for (int i=1; i<steps; i++) {
			std::vector<uint8_t>& src = back ? line2 : line1;
			std::vector<uint8_t>& dst = back ? line1 : line2;
			nextRow(src, dst);
			cnt += countSafe(dst);
			back = !back;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2016/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char ch;
		std::vector<uint8_t> line1;
		std::vector<uint8_t> line2;
		while (inputStream >> ch)
		{
			line1.push_back(ch == '^');
		}
		inputStream.close();
		bool back = false;
		int cnt = countSafe(line1);
		for (int i=1; i<steps2; i++) {
			std::vector<uint8_t>& src = back ? line2 : line1;
			std::vector<uint8_t>& dst = back ? line1 : line2;
			nextRow(src, dst);
			cnt += countSafe(dst);
			back = !back;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/