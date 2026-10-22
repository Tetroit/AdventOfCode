#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//pulling colored cubes from bag

struct Task
{
	static inline const std::unordered_map<std::string, int> pool{
		{"red", 12},
		{"green", 13},
		{"blue", 14},
	};
	static void run() {

		std::ifstream inputStream("2023/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int round = 0;
		int correct = 0;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			std::string token;
			int num, id;
			char shit;
			ss >> token;
			ss >> id >> shit;
			bool isCorrect = true;
			while (!ss.eof()) {
				ss >> num >> token;
				if (token.ends_with(',')) token.pop_back();
				else if (token.ends_with(';')) token.pop_back();
				int limit = pool.at(token);
				if (limit < num) {
					isCorrect = false;
					break;
				}
			}
			if (!isCorrect) continue;
			correct+=id;
		}
		inputStream.close();
		std::cout << correct << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2023/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int round = 0;
		int correct = 0;
		while (std::getline(inputStream, line))
		{
			std::unordered_map<std::string, int> max = {{"red", 0}, {"green", 0}, {"blue", 0}};
			std::stringstream ss(line);
			std::string token;
			int num, id;
			char shit;
			ss >> token;
			ss >> id >> shit;
			while (!ss.eof()) {
				ss >> num >> token;
				if (token.ends_with(',') || token.ends_with(';')) token.pop_back();
				if (num > max.at(token)) max.at(token) = num;
			}
			int score = 1;
			for (auto& val : max | std::views::values) {
				score *= val;
			}
			correct += score;
		}
		inputStream.close();
		std::cout << correct << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/