#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <map>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Calculation string hashes

struct Task
{
	static inline std::vector<std::string> strings;

	static int hash(const std::string& str) {
		int val=0;
		for (int i=0; i<str.size(); i++) {
			val = (val + str[i]) * 17 % 256;
		}
		return val;
	}
	static void run() {

		std::ifstream inputStream("2023/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			strings = Utils::split(line, ',');
		}
		inputStream.close();

		int sum = 0;
		for (auto& str : strings) {
			sum += hash(str);
		}
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		std::vector<std::vector<std::pair<std::string, int>>> boxes(256);
		for (auto str : strings) {
			if (str.back() == '-') {
				str.pop_back();
				auto& box = boxes[hash(str)];
				std::erase_if(box, [&str](auto& pair) {return pair.first == str;});
			}
			else {
				int val = 0;
				for (int i=str.find('=')+1; i < str.size(); i++) {
					val = val * 10 + (str[i] - '0');
				}
				str = str.substr(0, str.find('='));
				auto& box = boxes[hash(str)];
				auto it = std::find_if(box.begin(), box.end(), [&str](auto& pair) {return pair.first == str;});
				if (it == box.end()) {box.emplace_back(str, val);}
				else it->second = val;
			}
		}
		int score = 0;
		for (int i=0; i<boxes.size(); i++) {
			auto& box = boxes[i];
			for (int j=0; j<box.size(); j++) {
				score += (i + 1) * (j+1) * box[j].second;
			}
		}
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/