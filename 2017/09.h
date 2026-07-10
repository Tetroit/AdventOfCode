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

// brackets with groups and garbage
struct Task
{
	static void run() {

		std::ifstream inputStream("2017/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int group = 0;
		bool garbage = false;
		bool cancel = false;

		char c;
		int score = 0;
		while (inputStream >> c)
		{
			if (garbage) {
				if (c == '!') {
					inputStream >> c;
					continue;
				}
				if (c == '>') garbage = false;
			}
			else {
				if (c == '{') group++;
				else if (c == '}') {
					score += group;
					group--;
				}
				if (c == '<') garbage = true;
			}
		}
		inputStream.close();

		std::cout << score << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int group = 0;
		bool garbage = false;
		bool cancel = false;

		char c;
		int score = 0;
		while (inputStream >> c)
		{
			if (garbage) {
				if (c == '!') {
					inputStream >> c;
					continue;
				}
				if (c == '>') garbage = false;
				else score++;
			}
			else {
				if (c == '{') group++;
				else if (c == '}') group--;
				if (c == '<') garbage = true;
			}
		}
		inputStream.close();

		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/