#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//reading packet

struct Task
{
	static void run() {

		std::ifstream inputStream("2022/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char symb;
		int inputN = 0;
		std::deque<char> buffer;
		while (inputStream >> symb)
		{
			inputN++;
			buffer.push_back(symb);
			if (buffer.size() > 4) {
				buffer.pop_front();
			}
			int duplicate = -1;
			for (int i=0; i<buffer.size()-1; i++) {
				if (buffer[i] == symb) {
					duplicate = i;
					break;
				}
			}
			if (duplicate != -1) {
				for (int i=0; i<=duplicate; i++) {
					buffer.pop_front();
				}
			}
			else if (buffer.size() == 4) {
				break;
			}
		}
		inputStream.close();
		std::cout << inputN << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2022/06.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char symb;
		int inputN = 0;
		std::deque<char> buffer;
		while (inputStream >> symb)
		{
			inputN++;
			buffer.push_back(symb);
			if (buffer.size() > 14) {
				buffer.pop_front();
			}
			int duplicate = -1;
			for (int i=0; i<buffer.size()-1; i++) {
				if (buffer[i] == symb) {
					duplicate = i;
					break;
				}
			}
			if (duplicate != -1) {
				for (int i=0; i<=duplicate; i++) {
					buffer.pop_front();
				}
			}
			else if (buffer.size() == 14) {
				break;
			}
		}
		inputStream.close();
		std::cout << inputN << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/