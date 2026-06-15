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

//Replacing molecules
struct Task
{
	static inline std::unordered_map<std::string, std::vector<std::string>> replacements;
	static inline std::unordered_map<std::string, int> reg;
	static inline std::string mainSeq;

	static void registerPath(std::string key, int steps) {
		auto loc = reg.find(key);
		if (loc != reg.end()) {
			reg[key] = std::min(loc->second, steps);
		}
		else {
			reg[key] = steps;
		}
	}
	static void computeAllBranches(std::string root) {
		for (int i = 0; i<root.length(); i++) {
			for (const auto& [key, options] : replacements) {
				if (root.substr(i, key.length()) == key) {
					for (const auto& option : options) {
						std::string temp = root;
						temp.erase(i, key.length());
						temp.insert(i, option);
						registerPath(temp, 1);
					}
				}
			}
		}
	}
	static void run() {

		std::ifstream inputStream("2015/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}


		std::string line;
		std::regex pattern(R"((.+) => (.+))");
		bool inputSeq = false;
		while (std::getline(inputStream, line))
		{
			if (line == "")
				inputSeq = true;
			if (!inputSeq) {
				std::smatch match;
				std::regex_search(line, match, pattern);
				auto loc = replacements.find(match[1]);
				if (loc != replacements.end()) {
					loc->second.push_back(match[2]);
				}
				else {
					replacements.emplace(match[1], std::vector<std::string>{match[2]});
				}
			}
			else {
				mainSeq = line;
			}
		}
		inputStream.close();
		computeAllBranches(mainSeq);
		std::cout << reg.size() << std::endl;
		reg.clear();
	}
	static void runPart2() {
		int steps = -1;
		for (int i=0; i<mainSeq.length(); i++) {
			if (std::isupper(mainSeq[i]))
				steps++;
			if (mainSeq.substr(i, 2) == "Ar") {
				steps -= 2;
			}
			if (mainSeq.substr(i, 1) == "Y") {
				steps -= 2;
			}
		}
		std::cout << steps << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

looking at input there is a pattern in all of the transformations
from them you can describe the amount of added atoms with following rules:

1. Ar and Y cannot expand into anything
2. Ar is always the end of the sequence
3. Every sequence that doesnt end with Ar ends adds 1 length (1 -> 2)
4. Every sequence that DOES end with Ar has length 4 + amount of 'Y' * 2
5. Y can only be obtained from sequences ending with Ar

*/