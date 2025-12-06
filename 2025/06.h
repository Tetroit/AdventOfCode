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

//Calculate the expressions with + or * but plot twist: they are vertical
struct Task
{
	static void run() {

		std::ifstream inputStream("2025/input/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::vector<long long>> input;
		std::vector<char> operators;
		int row = 0;
		while (std::getline(inputStream, line))
		{
			if (line[0] == '*' || line[0] == '+') {
				for (char i : line) {
					if (i == '*') {
						operators.push_back('*');
					}
					if (i == '+')
						operators.push_back('+');
				}
				break;
			}
			input.emplace_back();
			std::stringstream ss(line);
			long long num;
			while (ss >> num) {
				input[row].push_back(num);
			}
			row++;
		}
		inputStream.close();
		long long sum = 0;
		for (int i=0; i<operators.size(); i++) {

			long long res = 0;
			switch (operators[i]) {
				case '+':
					res = 0;
					for (auto & j : input) {
						res += j[i];
					}
					break;
				case '*':
					res = 1;
					for (auto & j : input) {
						res *= j[i];
					}
					break;
				default:
					std::cerr << "Unrecognized operator " << operators[i] << "\n";
					break;
			}
			sum += res;
			if (sum < 0) {
				std::cerr << "ll overflow\n";
			}
		}
		std::cout << sum << "\n";
		//13440805474855688
	}
	static void runPart2() {

		std::ifstream inputStream("2025/input/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::string> rawInput;
		std::vector<std::vector<long long>> input;
		std::vector<char> operators;
		std::vector<int> cellWidths;

		while (std::getline(inputStream, line))
		{
			rawInput.emplace_back(line);
		}
		inputStream.close();

		auto& operatorsLine = rawInput.back();
		int currentWidth = 0;
		int totalWidth = operatorsLine.size();
		for (char i : operatorsLine) {
			if (i == '*') {
				if (!operators.empty())
					cellWidths.emplace_back(currentWidth);
				currentWidth=0;
				operators.push_back('*');
			}
			else if (i == '+') {
				if (!operators.empty())
					cellWidths.emplace_back(currentWidth);
				currentWidth=0;
				operators.push_back('+');
			}
			else {
				currentWidth++;
			}
		}
		int maxWidth = 0;
		for (int i=0; i<rawInput.size()-1; i++) {
			if (rawInput[i].size() > maxWidth) {
				maxWidth = rawInput[i].size();
			}
		}
		cellWidths.push_back(maxWidth - totalWidth + 1);

		/*
		 *|---j---|----j----|...
		 *| 1 2 3 | 2 3 4 2 |
		 *|   1 2 k 3 4     k
		 *|     1 | 4 5     |
		 *|-------|---------|
		 *|i=1    |i=2      |
		 *|op=+   |op=*     |
		 *|w=3    |w=4      |
		 */
		int x = 0;
		for (int i = 0; i<operators.size(); i++) {
			input.emplace_back();
			for (int j=0; j<cellWidths[i]; j++) {
				input[i].emplace_back(0);
				for (int k=0; k<rawInput.size()-1; k++) {
					if (rawInput[k].size() <= x || rawInput[k][x] == ' ') {
						continue;
					}
					input[i][j] = input[i][j] * 10 + static_cast<long long>(rawInput[k][x] - '0');
				}
				x++;
			}
			x++;
		}
		long long sum = 0;

		for (int i=0; i<operators.size(); i++) {
			long long res = 0;
			switch (operators[i]) {
				case '+':
					res = 0;
					for (auto & j : input[i]) {
						res += j;
					}
					break;
				case '*':
					res = 1;
					for (auto & j : input[i]) {
						res *= j;
					}
					break;
				default:
					std::cerr << "Unrecognized operator " << operators[i] << "\n";
					break;
			}
			sum += res;
			std::cout << res << "\n";
			if (sum < 0) {
				std::cerr << "ll overflow\n";
			}
		}
		std::cout << sum << "\n";
		//10951882745757
	}
};

//-------------- NOTES AREA ----------------
/*

*/