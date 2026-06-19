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

//real and decoy rooms


struct Room {
	int id;
	std::string letters;
	std::string cypher;
	std::string key;

	void Preprocess() {
		for (int i=letters.length() - 1; i>=0; i--) {
			if (letters[i] == '-') {
				letters.erase(i, 1);
			}
		}
	}
	bool Check() {
		std::unordered_map<char, int> freq;
		for (auto& ch : letters) {
			freq[ch]++;
		}
		std::vector<std::pair<char, int>> pairs;
		for (auto pair : freq) {
			pairs.emplace_back(pair);
		}
		std::ranges::sort(pairs, [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
			if (a.second == b.second) {
				return a.first < b.first;
			};
			return a.second > b.second;
		});
		for (int i=0; i<key.length(); i++) {
			if (key[i] != pairs[i].first) {
				return false;
			}
		}
		return true;
	}
	void PrintUncypher() {
		for (int i=0; i<id; i++) {
			for (char& c : cypher) {
				if (c == '-')
					c = ' ';
				else if (c == 'z')
					c='a';
				else if (c == ' ');
				else
					c++;
			}
		}
		std::cout << cypher << " " << id << std::endl;
	}
};

struct Task
{
	static inline std::vector<Room> rooms;
	static void run() {

		std::ifstream inputStream("2016/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int sum = 0;
		std::regex pattern(R"((.*)-(\d+)\[(\w+)\])");
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_search(line, match, pattern);
			Room r = {
				std::stoi(match[2]),
				match[1],
				match[1],
				match[3],
			};
			r.Preprocess();
			if (r.Check()) {
				sum += r.id;
			}
			rooms.push_back(r);
		}
		inputStream.close();
		std::cout << sum << std::endl;
	}
	static void runPart2() {
		for (auto& room : rooms) {
			room.PrintUncypher();
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/