#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <map>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//calculator with brackets


struct Task {

	static void apply(long long & a, long long  b, char op) {
		if (op == '+') a += b;
		else if (op == '*') a *= b;
	}
	static long long  calculate(std::string line, int& i) {

		int start = i;
		int end;
		long long val;
		if (line [i] == '(') {
			i++;
			val = calculate(line,i);
		}
		else {
			end = line.find(' ', i);
			val = std::stoll(line.substr(start, end - start));
			i = end;
		}

		while (i < line.size()) {
			i++;
			char op = line[i];
			i+=2;
			if (line[i] =='(') {
				i++;
				apply(val, calculate(line, i), op);
				if (i >= line.size()) return val;
				if (line[i] == ')') {
					i++;
					return val;
				}
				continue;
			}
			start = i;
			end = line.find(' ', i);
			if (end == -1) end = line.size();
			if (line[end-1] == ')') {
				end = line.find(')', i);
				apply(val, std::stoll(line.substr(start, end - start)), op);
				i=end + 1;
				return val;
			}
			apply(val, std::stoll(line.substr(start, end - start)), op);
			i=end;
		}
		return val;
	}
	static void run() {

		std::ifstream inputStream("2020/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		long long cnt = 0;
		while (std::getline(inputStream, line)) {
			int i=0;
			long long val = calculate(line, i);
			cnt += val;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}

	static void runPart2() {
		std::ifstream inputStream("2020/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		long long cnt = 0;
		while (std::getline(inputStream, line)) {
			std::string formula;
			std::vector<int> priorities;
			std::vector<long long> nums;
			std::map<std::string, int> map;
			int currentIndent = 0;
			int maxPriority = 0;
			for (char symb : line) {
				if (symb == '(') {
					currentIndent++;
				}
				else if (symb == ')') {
					currentIndent--;
				}
				if (symb == '+' || symb == '*') {
					formula += symb;
					int signVal = symb == '+' ? 1 : 0;
					int priority = currentIndent * 2 + signVal;
					priorities.push_back(priority);
					if (maxPriority < priority) {
						maxPriority = priority;
					}
				}
			}
			for (int i=0; i<line.size(); ++i) {
				int start, end;
				if (std::isdigit(line[i])) {
					start = i;
					end = i;
					while (end < line.size() && std::isdigit(line[end])) end++;
					nums.push_back(std::stoll(line.substr(start, end - start)));
					i = end;
				}
			}
			for (int i=maxPriority; i>=0; --i) {
				for (int op = formula.size()-1; op >= 0; --op) {
					if (priorities[op] == i) {
						apply(nums[op], nums[op+1], formula[op]);
						nums.erase(nums.begin() + op + 1);
						formula.erase(formula.begin() + op);
						priorities.erase(priorities.begin() + op);
					}
				}
			}
			cnt += nums[0];
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	thats some cool stuff on grammar consumption, I wanted to explore different approaches,
	so part 2 is not following parsing technique, but the idea is clear
*/