#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//deducing activation code by reordering and getting max value
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
		static bool execute(std::vector<int>& arr, int& ptr, std::deque<int>& input, std::deque<int>& output) {
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
				arr[arr[ptr+1]] = input.front();
				input.pop_front();
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

	static inline std::vector<int> program;

	static int runAmp(std::vector<int>& software, int setting, int signal, bool setPhase = true) {
		std::deque<int> input;
		if (setPhase) input.push_back(setting);
		input.push_back(signal);
		std::deque<int> output;
		int ptr = 0;
		while (execute(software, ptr, input, output));
		return output[0];
	}
	static int runTillOut(std::vector<int>& software, int& ptr, int setting, int signal, bool& halted, bool setPhase) {
		std::deque<int> input;
		if (setPhase) {
			input.push_back(setting);
		}
		input.push_back(signal);
		std::deque<int> output;
		bool exit = false;
		bool outputOp = false;
		do {
			exit = outputOp;
			if (exit)
				break;
			if (software[ptr]%100 == 4)
				outputOp = true;
		} while (execute(software, ptr, input, output));
		halted = !exit;
		if (halted) return signal;
		return output[0];
	}
	static int runLoop(std::array<std::vector<int>, 5>& wiring, const std::array<int,5> settings, int input) {
		int outSignal = input;
		for (int i=0; i<5; i++) {
			outSignal = runAmp(wiring.at(i), settings.at(i), outSignal);
		}
		return outSignal;
	}
	static int runFeedbackLoop(std::array<std::vector<int>, 5>& wiring, const std::array<int,5> settings, std::array<int, 5>& ptrs, bool& firstLoop, bool& halts, int input) {
		int outSignal = input;
		for (int i=0; i<5; i++) {
			outSignal = runTillOut(wiring.at(i), ptrs.at(i), settings.at(i), outSignal, halts, firstLoop);
			if (halts) return input;
		}
		if (firstLoop) firstLoop = false;
		return outSignal;
	}
	static void run() {

		std::ifstream inputStream("2019/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			program.push_back(num);
		}
		inputStream.close();

		std::array<int, 5> perm;
		std::iota(perm.begin(), perm.end(), 0);
		int max = 0;
		do {
			std::array<std::vector<int>, 5> wiring {program,program,program,program, program};
			int outSignal = runLoop(wiring, perm, 0);
			if (outSignal > max) max = outSignal;
		} while (std::next_permutation(perm.begin(), perm.end()));
		std::cout << max << std::endl;
	}
	static void runPart2() {
		int max = 0;
		std::array<int, 5> perm;
		std::iota(perm.begin(), perm.end(), 5);
		do {
			std::unordered_set<int> cache;
			std::array<int, 5> ptrs{0,0,0,0,0};
			std::array<std::vector<int>, 5> wiring {program,program,program,program, program};
			int outSignal = 0;
			bool firstLoop = true;
			bool halted = false;
			while (true) {
				outSignal = runFeedbackLoop(wiring, perm, ptrs, firstLoop, halted, outSignal);
				if (halted) break;
			}
			if (outSignal > max) max = outSignal;
		} while (std::next_permutation(perm.begin(), perm.end()));
		std::cout << max << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/