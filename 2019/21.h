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

#include "Intcode.h"
#include "utils.h"

//Intcode google dino game with custom bool scripting
struct Task
{
	static inline Intcode<int> program;
	static void stringReinterpret(std::deque<int>& input, std::string text) {
		for (int i=0; i<text.length(); i++) {
			input.push_back(static_cast<int>(text[i]));
		}
		input.push_back(10);
	}
	static void run() {

		std::ifstream inputStream("2019/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		program = Intcode<int>(inputStream);

		inputStream.close();

		std::deque<int> input;
		std::deque<int> output;
		HaltCode c;
		//jump only if D is ground and if there is need for it (A or B or C is gap)
		std::string springScript =
			"NOT A J\n"
			"NOT B T\n"
			"OR T J\n"
			"NOT C T\n"
			"OR T J\n"
			"AND D J\n"
			"WALK";
		stringReinterpret(input, springScript);
		program.start();
		while (true) {
			while (program.executeUntilInput(input, output, c));
			while (!output.empty()) {
				if (output.front() > CHAR_MAX)
					std::cout << output.front() << std::endl;
				else
					std::cout << (char)output.front();
				output.pop_front();
			}
			if (c == HaltCode::EXIT) break;
		}
		program.end();
	}
	static void runPart2() {
		std::deque<int> input;
		std::deque<int> output;
		HaltCode c;
		//same logic but jump only if possible to make next move, that is if we can walk or make another jump,
		//meaning if either 5 or 8 is ground
		std::string springScript =
			"NOT A J\n"
			"NOT B T\n"
			"OR T J\n"
			"NOT C T\n"
			"OR T J\n"
			"AND D J\n"

			"NOT E T\n"
			"AND H T\n"
			"OR E T\n"
			"AND T J\n"
			"RUN";
		stringReinterpret(input, springScript);
		program.start();
		while (true) {
			while (program.executeUntilInput(input, output, c));
			while (!output.empty()) {
				if (output.front() > CHAR_MAX)
					std::cout << output.front() << std::endl;
				else
					std::cout << (char)output.front();
				output.pop_front();
			}
			if (c == HaltCode::EXIT) break;
		}
		program.end();
	}
};

//-------------- NOTES AREA ----------------
/*

*/