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

//opening and closes brackets of all kinds and shapes

struct Task
{
	static bool isOpen (char c){
		return c == '(' || c == '{' || c == '[' || c == '<';
	}
	static bool match (char open, char close) {
		return
			open == '(' && close == ')' ||
			open == '{' && close == '}' ||
			open == '[' && close == ']' ||
			open == '<' && close == '>';
	}
	static inline const std::unordered_map<char, int> scores = {
		{')', 3},
		{']', 57},
		{'}', 1197},
		{'>', 25137}
	};
	static inline const std::unordered_map<char, int> scores2 = {
		{'(', 1},
		{'[', 2},
		{'{', 3},
		{'<', 4}
	};

	static inline std::vector<std::string> lines;
	static inline std::vector<int> incomplete;
	static void run() {
		std::ifstream inputStream("2021/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int score = 0;
		int lineN = 0;
		while (std::getline(inputStream, line))
		{
			lines.push_back(line);
			std::vector<char> stack;
			bool error = false;
			for (char i : line) {
				if (isOpen(i)) {
					stack.push_back(i);
				}
				else {
					if (match(stack.back(), i)) {
						stack.pop_back();
					}
					else {
						score += scores.at(i);
						error = true;
						break;
					}
				}
			}
			if (!error) {incomplete.push_back(lineN);}
			lineN ++;
		}
		inputStream.close();
		std::cout << score << std::endl;
	}
	static void runPart2() {
		std::vector<long long> lineScores;
		for (auto i : incomplete) {
			std::vector<char> stack;
			for (auto c : lines[i]) {
				if (isOpen(c)) {
					stack.push_back(c);
				}
				else {
					if (match(stack.back(), c)) {
						stack.pop_back();
					}
				}
			}
			long long score = 0;
			for (int symb=stack.size()-1; symb>=0; symb--) {
				char c = stack[symb];
				score = score * 5 + scores2.at(c);
			}
			lineScores.push_back(score);
		}
		std::sort(lineScores.begin(), lineScores.end());
		std::cout << lineScores[lineScores.size()/2] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/