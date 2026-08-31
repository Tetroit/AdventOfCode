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
#include "Intcode.h"

//we playing games now yippee

struct Task
{
	static inline Intcode<long long> program;
	static void stringReinterpret(std::deque<long long>& input, std::string text) {
		for (int i=0; i<text.length(); i++) {
			input.push_back(text[i]);
		}
		input.push_back(10);
	}
	static void run() {

		std::ifstream inputStream("2019/25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		program = Intcode<long long>(inputStream);

		inputStream.close();

		std::deque<long long> input;
		std::deque<long long> output;
		HaltCode c;
		std::string console;
		while (true) {
			while (program.executeUntilInput(input, output, c));
			auto it = output.begin();
			while (it != output.end()) {
				auto val = *it;
				if (val <= CHAR_MAX)
					std::cout << (char)*it;
				else
					std::cout << val;
				it++;
			}
			std::cout << std::endl;
			output.clear();
			std::getline(std::cin, console);
			stringReinterpret(input, console);
		}
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*



*/