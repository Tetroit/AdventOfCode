#pragma once

#include <algorithm>
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

//duplicate words
struct Task
{
	static void run() {

		std::ifstream inputStream("2017/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt = 0;
		while (std::getline(inputStream, line))
		{
			std::vector<std::string> tokens = Utils::split(line, " ");
			std::unordered_set<std::string> unique;
			for (auto tok : tokens) {
				unique.insert(tok);
			}
			if (tokens.size() == unique.size()) cnt++;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt = 0;
		while (std::getline(inputStream, line))
		{
			std::vector<std::string> tokens = Utils::split(line, " ");
			std::unordered_set<std::string> unique;
			for (auto tok : tokens) {
				std::sort(tok.begin(), tok.end());
				unique.insert(tok);
			}
			if (tokens.size() == unique.size()) cnt++;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/