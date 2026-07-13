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

// "dancing" with letters

using Program = std::vector<std::function<void(std::string&)>>;
struct Task
{
	static constexpr std::string order(int x) {
		std::string order;
		for (int i=0; i<x; i++) {
			order += ('a' + i);
		}
		return order;
	}

	static inline Program prog;
	static void run() {

		std::ifstream inputStream("2017/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string str = order(16);
		char op;
		while (inputStream >> op)
		{
			int arg1, arg2;
			char arg1Ch, arg2Ch;
			if (op == 's') {
				inputStream >> arg1;
				inputStream.ignore();
				prog.emplace_back([arg1](std::string& str) {
					std::string rest = str.substr(str.length() - arg1, arg1);
					str.erase(str.end() - arg1, str.end());
					str.insert(0, rest);
				});
			}
			else if (op == 'x') {
				inputStream >> arg1;
				inputStream.ignore();
				inputStream >> arg2;
				inputStream.ignore();
				prog.emplace_back([arg1, arg2](std::string& str) {
					std::swap(str[arg1], str[arg2]);
				});
			}
			else if (op == 'p') {
				inputStream >> arg1Ch;
				inputStream.ignore();
				inputStream >> arg2Ch;
				inputStream.ignore();
				prog.emplace_back([arg1Ch, arg2Ch](std::string& str) {
					std::swap(str[str.find(arg1Ch)], str[str.find(arg2Ch)]);
				});
			}
		}
		inputStream.close();
		for (auto& line : prog) {
			line(str);
		}
		std::cout << str << std::endl;
	}
	static void runPart2() {
		const int iterations = 1000000000;
		std::string str = order(16);
		int loop = -1;
		for (int i=0; i<iterations; i++) {
			for (auto& line : prog) {
				line(str);
			}
			if (str == order(16)) {
				loop = i + 1;
				std::cout << "loop found after " << loop << " iterations" << std::endl;
				break;
			}
		}
		if (loop != -1) {
			for (int i=0; i < iterations % loop; i++) {
				for (auto& line : prog) {
					line(str);
				}
			}
		}
		std::cout << str << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/