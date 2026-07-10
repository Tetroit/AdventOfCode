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

//conditional increase decrease operations

struct Task
{
	static inline std::unordered_map<std::string, int> regist;
	static void run() {

		std::ifstream inputStream("2017/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string reg;

		int max = std::numeric_limits<int>::min();
		while (inputStream >> reg)
		{
			std::string compReg, compOp, op, shit;
			int compVal, val;
			inputStream >> op >> val >> shit >> compReg >> compOp >> compVal;
			auto pred = [&compOp](const int& a, const int& b) {
				if (compOp == ">")
					return a > b;
				if (compOp == "<")
					return a < b;
				if (compOp == "<=")
					return a <= b;
				if (compOp == ">=")
					return a >= b;
				if (compOp == "==")
					return a == b;
				return a != b;
			};
			auto opLambda = [op](int& a, const int& b) {
				if (op == "dec") {
					a -= b;
				}
				if (op == "inc") {
					a += b;
				}
			};
			if (pred(regist[compReg], compVal))
				opLambda(regist[reg], val);

		}
		inputStream.close();

		for (auto& [address, val] : regist) {
			if (val > max) {
				max = val;
			}
		}
		std::cout << max << std::endl;
	}
	static void runPart2() {

		std::ifstream inputStream("2017/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string reg;
		regist.clear();

		int max = std::numeric_limits<int>::min();
		while (inputStream >> reg)
		{
			std::string compReg, compOp, op, shit;
			int compVal, val;
			inputStream >> op >> val >> shit >> compReg >> compOp >> compVal;
			auto pred = [&compOp](const int& a, const int& b) {
				if (compOp == ">")
					return a > b;
				if (compOp == "<")
					return a < b;
				if (compOp == "<=")
					return a <= b;
				if (compOp == ">=")
					return a >= b;
				if (compOp == "==")
					return a == b;
				return a != b;
			};
			auto opLambda = [op](int& a, const int& b) {
				if (op == "dec") {
					a -= b;
				}
				if (op == "inc") {
					a += b;
				}
			};
			if (pred(regist[compReg], compVal)) {
				opLambda(regist[reg], val);
				if (regist[reg] > max) {
					max = regist[reg];
				}
			}
		}
		inputStream.close();
		std::cout << max << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

*/