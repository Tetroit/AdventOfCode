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

//intcode computer AGAIN but with offsets

template <typename T>
struct Intcode {
	std::vector<T> program;
	std::vector<T> reserve;
	int ptr=0;
	int base=0;

	void start() {
		reserve = program;
	}
	void end() {
		program = reserve;
		ptr = 0;
		base = 0;
	}
	static void decypher(int instruction, int& opcode, int& mode1, int& mode2, int& mode3) {
		opcode = instruction % 100;
		instruction /= 100;
		mode1 = instruction % 10;
		instruction /= 10;
		mode2 = instruction % 10;
		instruction /= 10;
		mode3 = instruction % 10;
	}
	T& get (T pos) {
		if (pos >= program.size()) {
			program.resize(pos + 1);
		}
		return program[pos];
	}
	T getByMode(T pos, int mode) {
		if (mode == 1) return pos;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	T& getRefByMode(T pos, int mode) {
		if (mode == 1) std::cerr << "Cannot write to rvalue" << std::endl;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	bool execute(std::deque<T>& input, std::deque<T>& output) {
		int mode1 = 0;
		int mode2 = 0;
		int mode3 = 0;
		int code = 0;
		decypher(get(ptr), code, mode1, mode2, mode3);
		if (code == 99) return false;
		switch (code) {
			case 1: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) + getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 2: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) * getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 3: {
				getRefByMode(get(ptr+1), mode1) = input.front();
				input.pop_front();
				ptr+=2;
				break;
			}
			case 4: {
				output.push_back(getByMode(get(ptr+1), mode1));
				ptr+=2;
				break;
			}
			case 5: {
				ptr = getByMode(get(ptr+1), mode1) != 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 6: {
				ptr = getByMode(get(ptr+1), mode1) == 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 7: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) < getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 8: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) == getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 9: {
				base += getByMode(get(ptr+1), mode1);
				ptr+=2;
				break;
			}
			default: {
				std::cout << "Unknown code: " << code << std::endl;
				return false;
			}
		}
		return true;
	}
};

struct Task
{


	static inline Intcode<long long> computer;

	static void run() {

		std::ifstream inputStream("2019/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		long long num;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			computer.program.push_back(num);
		}
		inputStream.close();
		std::deque<long long> input{1};
		std::deque<long long> output;
		computer.start();
		while (computer.execute(input, output)) {
			while (!output.empty()) {
				std::cout << output.front() << std::endl;
				output.pop_front();
			}
		}
		computer.end();
	}
	static void runPart2() {
		std::deque<long long> input{2};
		std::deque<long long> output;
		computer.start();
		while (computer.execute(input, output)) {
			while (!output.empty()) {
				std::cout << output.front() << std::endl;
				output.pop_front();
			}
		}
		computer.end();
	}
};

//-------------- NOTES AREA ----------------
/*

*/