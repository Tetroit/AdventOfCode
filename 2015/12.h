#pragma once

#include <charconv>
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
#include "../utils.h"

//The task about parsing json for numbers and then without red objects

struct Task
{
	static void run() {

		std::ifstream inputStream("12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::regex numberPattern(R"((-?\d+))");
		int sum = 0;
		while (std::getline(inputStream, line))
		{
			auto begin = std::sregex_iterator(line.begin(), line.end(), numberPattern);
			auto end = std::sregex_iterator();
			for (auto i = begin; i != end; i++) {

				std::smatch match = *i;
				sum += std::stoi(match[1].str());
			}
		}
		inputStream.close();
		std::cout << sum << '\n';
	}
	static std::vector<std::pair<int,int>> analyseObject(const std::string& input, int start, int end, bool& foundRed, int& sum) {
		std::vector<std::pair<int,int>> output;
		sum = 0;
		int brackets = 0;
		int startCut = -1;
		for (int i=start; i <= end; i++) {
			if (brackets == 0 && i+5 <= end && input.substr(i, 6) == ":\"red\"") {
				foundRed = true;
			}
			if (brackets == 0 && (input[i] == '-' || std::isdigit(input[i]))) {
				int num = 0;
				auto numEnd = std::from_chars(input.data() + i, input.data() + input.size(), num);
				if (numEnd.ec == std::errc()) {
					sum += num;
					i = numEnd.ptr - input.data() - 1;
				}
			}
			if (input[i] == '{') {
				if (brackets == 0) {
					startCut = i+1;
				}
				brackets++;
			}
			if (input[i] == '}') {
				brackets--;
				if (brackets == 0) {
					output.emplace_back(startCut, i-1);
				}
			}
		}
		return output;
	}
	static void runPart2() {

		std::ifstream inputStream("12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::regex numberPattern(R"((-?\d+))");
		int sum = 0;
		std::getline(inputStream, line);

		std::vector<std::pair<int,int>> toCheck{std::make_pair(0, line.size() - 1)};
		while (!toCheck.empty()) {
			auto [start, end] = toCheck.back();
			bool foundRed = false;
			int objectNum = 0;
			auto children = analyseObject(line, start, end, foundRed, objectNum);
			toCheck.pop_back();
			if (!foundRed) {
				sum += objectNum;
				for (auto & [s,e] : children) {
					toCheck.emplace_back(s, e);
				}
				std::cout << "okay: " << line.substr(start, end - start + 1) << '\n';
			}
			else {
				std::cout << "RED: " << line.substr(start, end - start + 1) << '\n';
			}
		}


		inputStream.close();
		std::cout << sum << '\n';
		//102840
	}
};

//-------------- NOTES AREA ----------------
/*
//damn... regex wont do for part 2
*/