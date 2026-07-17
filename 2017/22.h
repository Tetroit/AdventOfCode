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

//infecting cells and turning right and left
struct Task
{
	enum struct State {
		CLEAN = 0,
		WEAKENED = 1,
		INFECTED = 2,
		FLAGGED = 3,
	};
	static inline std::unordered_set<uint32_t> map;
	static inline std::unordered_map<uint64_t, State> map2;
	static void run() {

		std::ifstream inputStream("2017/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int row = 0;
		while (std::getline(inputStream, line))
		{
			for (int i=0; i<line.length(); i++) {
				if (line[i] == '#') {
					map.emplace(uint32_t(uint16_t(row)) << 16 | uint32_t(uint16_t(i)));
				}
			}
			row++;
		}
		int inWidth = line.length();
		int inHeight = row;
		int dir = 1;
		int16_t x = inWidth/2;
		int16_t y = inHeight/2;
		inputStream.close();
		int cnt = 0;
		for (int i=0; i<10000; i++) {
			uint32_t key = uint32_t(uint16_t(y)) << 16 | uint32_t(uint16_t(x));
			if (map.contains(key)) {
				dir = Utils::Loop(dir - 1, 0 , 3);
				map.erase(key);
			}
			else {
				dir = Utils::Loop(dir + 1, 0 , 3);
				map.emplace(key);
				cnt++;
			}
			auto [dx, dy] = Utils::UtilFacingVec[dir];
			x += int16_t(dx);
			y += int16_t(dy);
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int row = 0;
		while (std::getline(inputStream, line))
		{
			for (int i=0; i<line.length(); i++) {
				if (line[i] == '#') {
					map2.emplace(uint64_t(uint32_t(row)) << 32 | uint64_t(uint32_t(i)), State::INFECTED);
				}
			}
			row++;
		}
		int inWidth = line.length();
		int inHeight = row;
		int dir = 1;
		int32_t x = inWidth/2;
		int32_t y = inHeight/2;
		inputStream.close();
		int cnt = 0;
		for (int i=0; i<10000000; i++) {
			uint64_t key = uint64_t(uint32_t(y)) << 32 | uint64_t(uint32_t(x));
			State state = map2[key];
			switch (map2[key]){
				case State::CLEAN: {
					dir = Utils::Loop(dir + 1, 0 , 3);
					break;
				}
				case State::WEAKENED: {
					cnt++;
					break;
				}
				case State::INFECTED: {
					dir = Utils::Loop(dir - 1, 0 , 3);
					break;
				}
				case State::FLAGGED: {
					dir = Utils::Loop(dir + 2, 0 , 3);
					break;
				}
			}
			map2[key] = (State)(((uint32_t)state + 1) % 4);
			auto [dx, dy] = Utils::UtilFacingVec[dir];
			x += int32_t(dx);
			y += int32_t(dy);
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/