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

//catching bus by schedule
struct Task
{
	static inline std::vector<int> buses;
	static inline std::vector<int> offsets;
	static void run() {

		std::ifstream inputStream("2020/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int ts;
		inputStream >> ts;
		inputStream.ignore(1);
		std::getline(inputStream, line);
		auto words = Utils::split(line, ',');
		for (int i=0; i < words.size(); i++) {
			const auto& word = words[i];
			try {
				int parsed = std::stoi(word);
				buses.push_back(parsed);
				offsets.push_back(i);
			}
			catch (const std::invalid_argument& e) {}
		}
		inputStream.close();

		int min = INT_MAX;
		int minID = -1;
		for (int i=0; i<buses.size(); i++) {
			const auto& bus = buses[i];
			if (ts%bus == 0) {
				min = 0;
				minID = bus;
				break;
			}
			int dist = bus - (ts%bus);
			if (dist < min) {
				min = dist;
				minID = bus;
			}
		}
		std::cout << min * minID << std::endl;
	}
	static void runPart2() {
		long long ts = 0;
		long long stride = buses[0];
		if (buses.size() < 2) {
			std::cout << 0 << std::endl;
			return;
		}
		for (int i=1; i<buses.size(); i++) {
			while (buses[i] - ts%buses[i] != offsets[i]%buses[i]) {
				ts+=stride;
			}
			stride = Utils::lcm(stride, (long long)buses[i]);
		}
		std::cout << ts << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/