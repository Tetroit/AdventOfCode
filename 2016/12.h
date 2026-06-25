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

//assembly simulator


struct Task
{
	static inline std::vector<std::function<void(int&)>> program;
	static inline std::unordered_map<char, int> reg;
	static void run() {

		std::ifstream inputStream("2016/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (inputStream >> line)
		{
			if (line == "cpy") {
				std::string src;
				char address;
				inputStream >> src >> address;
				if (src[0] >= '0' && src[0] <= '9') {
					int val = std::stoi(src);
					program.emplace_back([address, val](int& pos) {
						reg[address] = val;
						pos++;
					});
				}
				else {
					char addressSrc = src[0];
					program.emplace_back([address, addressSrc](int& pos) {
						reg[address] = reg[addressSrc];
						pos++;
					});
				}
			}
			if (line == "jnz") {
				std::string src;
				int stride;
				inputStream >> src >> stride;
				if (src[0] >= '0' && src[0] <= '9') {
					int val = std::stoi(src);
					program.emplace_back([stride, val](int& pos) {
						if (val == 0) {
							pos++;
							return;
						}
						pos+=stride;
					});
				}
				else {
					char addressSrc = src[0];
					program.emplace_back([stride, addressSrc](int& pos) {
						if (reg[addressSrc] == 0) {
							pos++;
							return;
						}
						pos+=stride;
					});
				}
			}
			if (line == "inc") {
				char address;
				inputStream >> address;
				program.emplace_back([address](int& pos) {
					reg[address]++;
					pos++;
				});
			}
			if (line == "dec") {
				char address;
				inputStream >> address;
				program.emplace_back([address](int& pos) {
					reg[address]--;
					pos++;
				});
			}
		}
		inputStream.close();
		int progPos = 0;
		reg['a'] = 0;
		reg['b'] = 0;
		reg['c'] = 0;
		reg['d'] = 0;
		while (progPos < program.size()) {
			program[progPos](progPos);
		}
		std::cout << reg['a'] << std::endl;
	}
	static void runPart2() {
		int progPos = 0;
		reg['a'] = 0;
		reg['b'] = 0;
		reg['c'] = 1;
		reg['d'] = 0;
		while (progPos < program.size()) {
			program[progPos](progPos);
		}
		std::cout << reg['a'] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/