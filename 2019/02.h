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

//opcodes in int array
struct Task
{
	static void run() {

		std::ifstream inputStream("2019/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> arr;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			arr.push_back(num);
		}
		inputStream.close();
		arr[1] = 12;
		arr[2] = 2;
		for (int i=0; i<arr.size(); i+=4) {
			if (arr[i] == 99) break;
			if (arr[i] == 1) {
				arr[arr[i+3]] = arr[arr[i+1]] + arr[arr[i+2]];
			}
			if (arr[i] == 2) {
				arr[arr[i+3]] = arr[arr[i+1]] * arr[arr[i+2]];
			}
		}
		std::cout << arr[0] << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2019/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		std::vector<int> init;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			init.push_back(num);
		}
		inputStream.close();
		for (int a = 0; a < 100; a++) {
			for (int b = 0; b < 100; b++) {
				std::vector<int> arr = init;
				arr[1] = a;
				arr[2] = b;
				for (int i=0; i<arr.size(); i+=4) {
					if (arr[i] == 99) break;
					if (arr[i] == 1) {
						arr[arr[i+3]] = arr[arr[i+1]] + arr[arr[i+2]];
					}
					if (arr[i] == 2) {
						arr[arr[i+3]] = arr[arr[i+1]] * arr[arr[i+2]];
					}
				}
				if (arr[0] == 19690720) {
					std::cout << a * 100 + b << std::endl;
					break;
				}
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/