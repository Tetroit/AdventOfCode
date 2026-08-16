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

//extended opcode on array (see day 2)
struct Task
{
	static void decypher(int instruction, int& opcode, int& mode1, int& mode2, int& mode3) {
		opcode = instruction % 100;
		instruction /= 100;
		mode1 = instruction % 10 == 1 ? 1 : 0;
		instruction /= 10;
		mode2 = instruction % 10 == 1 ? 1 : 0;
		instruction /= 10;
		mode3 = instruction % 10 == 1 ? 1 : 0;
	}
	static bool execute(std::vector<int>& arr, int& ptr, std::vector<int>& input, std::vector<int>& output) {
		const auto getByMode = [&arr](int pos, int mode) {
			if (mode == 1) return pos;
			else return arr[pos];
		};
		int mode1 = 0;
		int mode2 = 0;
		int mode3 = 0;
		int code = 0;
		decypher(arr[ptr], code, mode1, mode2, mode3);
		if (code == 99) return false;
		switch (code) {
			case 1: {
				arr[getByMode(ptr+3, mode3)] = arr[getByMode(ptr+1, mode1)] + arr[getByMode(ptr+2, mode2)];
				ptr+=4;
				break;
			}
			case 2: {
				arr[getByMode(ptr+3, mode3)] = arr[getByMode(ptr+1, mode1)] * arr[getByMode(ptr+2, mode2)];
				ptr+=4;
				break;
			}
			case 3: {
				arr[arr[ptr+1]] = input.back();
				input.pop_back();
				ptr+=2;
				break;
			}
			case 4: {
				output.push_back(arr[getByMode(ptr+1, mode1)]);
				ptr+=2;
				break;
			}
			case 5: {
				ptr = arr[getByMode(ptr+1, mode1)] != 0 ? arr[getByMode(ptr+2, mode2)] : ptr+3;
				break;
			}
			case 6: {
				ptr = arr[getByMode(ptr+1, mode1)] == 0 ? arr[getByMode(ptr+2, mode2)] : ptr+3;
				break;
			}
			case 7: {
				arr[arr[ptr+3]] = arr[getByMode(ptr+1, mode1)] < arr[getByMode(ptr+2, mode2)] ? 1 : 0;
				ptr+=4;
				break;
			}
			case 8: {
				arr[arr[ptr+3]] = arr[getByMode(ptr+1, mode1)] == arr[getByMode(ptr+2, mode2)] ? 1 : 0;
				ptr+=4;
				break;
			}
			default: {
				std::cout << "Unknown code: " << code << std::endl;
				return false;
			}
		}
		return true;
	}
	static inline std::vector<int> arr;
	static void run() {

		std::ifstream inputStream("2019/05.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			arr.push_back(num);
		}
		inputStream.close();

		auto copy = arr;
		std::vector<int> input{1};
		std::vector<int> output;
		int ptr = 0;
		while (execute(copy, ptr, input, output));
		for (int i = 0; i < output.size(); i++) {
			std::cout << i << ":\t" << output[i] << '\n';
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		auto copy = arr;
		std::vector<int> input{5};
		std::vector<int> output;
		int ptr = 0;
		while (execute(copy, ptr, input, output));
		for (int i = 0; i < output.size(); i++) {
			std::cout << i << ":\t" << output[i] << '\n';
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/