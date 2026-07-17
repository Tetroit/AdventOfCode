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

//running state machine on tape

// changed input format to
// A 12302209
// A: 1 r B 0 l D
struct State {
	bool set0;
	char dir0;
	char next0;

	bool set1;
	char dir1;
	char next1;
};
struct Task
{
	static inline std::unordered_map<char, State> states;
	static inline std::unordered_map<int, bool> cells;
	static void run() {

		std::ifstream inputStream("2017/25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int checkStep;
		char currentState;
		inputStream >> currentState >> checkStep;
		char stateName;
		while (inputStream >> stateName)
		{
			inputStream.ignore(1);
			State state;
			inputStream >>
			state.set0 >> state.dir0 >> state.next0 >>
			state.set1 >> state.dir1 >> state.next1;

			states[stateName] = state;
		}
		inputStream.close();
		int pos = 0;
		for (int i=0; i<checkStep; i++) {
			State& state = states[currentState];
			if (cells[pos] == 0) {
				cells[pos] = state.set0;
				pos += state.dir0 == 'r' ? 1 : -1;
				currentState = state.next0;
			}
			else {
				cells[pos] = state.set1;
				pos += state.dir1 == 'r' ? 1 : -1;
				currentState = state.next1;
			}
		}
		int cnt = 0;
		for (auto& [pos, val] : cells) {
			if (val == 1) cnt++;
		}
		std::cout << cnt << '\n';
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/