#pragma once

#include <array>
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
#include "Intcode.h"

//Computer network communication

struct Interface {
	std::deque<long long> input;
	std::deque<long long> output;
	Intcode<long long> program;
	HaltCode executeUntilInput() {
		HaltCode retVal;
		while (program.executeUntilInput(input, output, retVal));
		return retVal;
	}
	void start() {program.start();}
	void end() {program.end();}
};
struct Task
{
	static constexpr int N = 50;
	static inline std::array<Interface, N> interfaces;

	static void distributePackets(bool& sent255, int& idleCounter, long long& natX, long long& natY) {
		long long dstN, x, y;
		for (int i=0; i<N; i++) {
			auto& src = interfaces[i];
			while (!src.output.empty()) {
				dstN = src.output.front();
				src.output.pop_front();
				x = src.output.front();
				src.output.pop_front();
				y = src.output.front();
				src.output.pop_front();
				if (dstN == 255) {
					natX = x;
					natY = y;
					sent255 = true;
					continue;
				}
				auto& dst = interfaces[dstN];
				dst.input.push_back(x);
				dst.input.push_back(y);
			}
		}
		int idleComputers = 0;
		for (int i=0; i<N; i++) {
			if (interfaces[i].input.empty()) {
				interfaces[i].input.push_back(-1);
				idleComputers++;
			}
		}
		if (idleComputers == N)
			idleCounter++;
		else
			idleCounter = 0;
	}
	static void run() {

		std::ifstream inputStream("2019/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		Intcode<long long> program = Intcode<long long>(inputStream);
		for (int i=0; i<N; i++) {
			auto& interface = interfaces[i];
			interface.program = program;
			interface.input.push_back(i);
			interface.start();
		}
		inputStream.close();

		bool sent255 = false;
		int idleCounter = 0;
		long long natX, natY;
		while (!sent255) {
			for (auto& i : interfaces) {
				HaltCode hc = i.executeUntilInput();
			}
			distributePackets(sent255, idleCounter, natX, natY);
			if (sent255) {
				std::cout << natY << std::endl;
			}
		}
		for (int i=0; i<N; i++) {
			auto& interface = interfaces[i];
			interface.input.clear();
			interface.output.clear();
			interface.end();
		}
	}
	static void runPart2() {
		for (int i=0; i<N; i++) {
			auto& interface = interfaces[i];
			interface.input.push_back(i);
			interface.start();
		}
		bool sent255 = false;
		int idleCounter = 0;
		std::unordered_set<long long> refreshYs;
		long long natX = 0, natY = 0;
		while (true) {
			if (idleCounter == 2) {
				idleCounter = 0;
				if (refreshYs.contains(natY)) {
					std::cout << natY << std::endl;
					break;
				}
				interfaces[0].input.push_back(natX);
				interfaces[0].input.push_back(natY);
				refreshYs.insert(natY);
			}
			for (auto& i : interfaces) {
				HaltCode hc = i.executeUntilInput();
			}
			distributePackets(sent255, idleCounter, natX, natY);
		}
		for (int i=0; i<N; i++) {
			auto& interface = interfaces[i];
			interface.input.clear();
			interface.output.clear();
			interface.end();
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/