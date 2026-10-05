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

//packing backpacks

struct Task
{
	static inline std::vector<std::string> rucksacks;
	static void run() {

		std::ifstream inputStream("2022/03.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<char> errors;
		while (std::getline(inputStream, line))
		{
			rucksacks.push_back(line);
			std::unordered_set<char> items1;
			for (int i=0; i<line.size()/2; i++) {
				items1.insert(line[i]);
			}
			std::unordered_set<char> common;
			for (int i=line.size()/2; i<line.size(); i++) {
				if (items1.contains(line[i])) {
					common.insert(line[i]);
				}
			}
			for (auto err : common) {
				errors.push_back(err);
			}
		}
		int score = 0;
		for (auto err : errors) {
			if (std::islower(err)) score += err - 'a' + 1;
			if (std::isupper(err)) score += err - 'A' + 27;
		}
		inputStream.close();
		std::cout << score << std::endl;
	}
	static void runPart2() {
		std::vector<char> badges;
		for (int i = 0; i < rucksacks.size(); i+=3) {
			std::unordered_set<char> common;
			std::unordered_set<char> common2;
			auto* from = &common;
			auto* to = &common2;
			for (auto ch : rucksacks[i]) {
				common.insert(ch);
			}
			for (int n = 1; n < 3; n++) {
				to->clear();
				for (auto ch : rucksacks[i+n]) {
					if (from->contains(ch))
						to->insert(ch);
				}
				std::swap(from, to);
			}
			for (auto ch : *from) {
				badges.push_back(ch);
			}
		}
		int score = 0;
		for (auto badge : badges) {
			if (std::islower(badge)) score += badge - 'a' + 1;
			if (std::isupper(badge)) score += badge - 'A' + 27;
		}
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/