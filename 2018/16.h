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

//assembly simulator but you dont know anything

enum struct Command {
	addr = 0,
	addi = 1,
	mulr = 2,
	muli = 3,
	banr = 4,
	bani = 5,
	borr = 6,
	bori = 7,
	setr = 8,
	seti = 9,
	gtir = 10,
	gtri = 11,
	gtrr = 12,
	eqir = 13,
	eqri = 14,
	eqrr = 15,
};
struct Task
{
	static void Execute(Command cmd, std::vector<int>& reg, int a, int b, int c) {
		switch (cmd) {
			case Command::addr: {
				reg[c] = reg[a] + reg[b];
				break;
			}
			case Command::addi: {
				reg[c] = reg[a] + b;
				break;
			}
			case Command::mulr: {
				reg[c] = reg[a] * reg[b];
				break;
			}
			case Command::muli: {
				reg[c] = reg[a] * b;
				break;
			}
			case Command::banr: {
				reg[c] = reg[a] & reg[b];
				break;
			}
			case Command::bani: {
				reg[c] = reg[a] & b;
				break;
			}
			case Command::borr: {
				reg[c] = reg[a] | reg[b];
				break;
			}
			case Command::bori: {
				reg[c] = reg[a] | b;
				break;
			}
			case Command::setr: {
				reg[c] = reg[a];
				break;
			}
			case Command::seti: {
				reg[c] = a;
				break;
			}
			case Command::gtir: {
				reg[c] = a > reg[b] ? 1 : 0;
				break;
			}
			case Command::gtri: {
				reg[c] = reg[a] > b ? 1 : 0;
				break;
			}
			case Command::gtrr: {
				reg[c] = reg[a] > reg[b] ? 1 : 0;
				break;
			}
			case Command::eqir: {
				reg[c] = a == reg[b] ? 1 : 0;
				break;
			}
			case Command::eqri: {
				reg[c] = reg[a] == b ? 1 : 0;
				break;
			}
			case Command::eqrr: {
				reg[c] = reg[a] == reg[b] ? 1 : 0;
				break;
			}
		}
	}

	static void run() {

		std::ifstream inputStream("2018/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<int> before(4,0);
		std::vector<int> after(4,0);
		int command, a,b,c;
		char shit;
		int cnt=0;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) break;
			{
				std::istringstream ss(line);
				ss.ignore(sizeof("Before: [")-1);
				for (int i=0; i<4; i++) {
					ss >> before[i] >> shit;
				}
			}
			std::getline(inputStream, line);
			{
				std::istringstream ss(line);
				ss >> command >> a >> b >> c;
			}
			std::getline(inputStream, line);
			{
				std::istringstream ss(line);
				ss.ignore(sizeof("After:  [")-1);
				for (int i=0; i<4; i++) {
					ss >> after[i] >> shit;
				}
			}
			std::getline(inputStream, line);

			int possible = 0;
			for (int i=0; i<16; i++) {
				auto temp = before;
				Execute((Command)i, temp, a, b, c);
				bool match = true;
				for (int i=0; i<4; i++) {
					if (temp[i] != after[i]) {
						match = false;
						break;
					}
				}
				if (match) {
					possible++;
				}
			}
			if (possible >= 3) cnt++;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {

		std::ifstream inputStream("2018/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::vector<std::unordered_map<Command, int>> probs(16);
		std::vector<Command> map(16);

		std::string line;
		std::vector<int> before(4,0);
		std::vector<int> after(4,0);
		int command, a,b,c;
		char shit;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) break;
			{
				std::istringstream ss(line);
				ss.ignore(sizeof("Before: [")-1);
				for (int i=0; i<4; i++) {
					ss >> before[i] >> shit;
				}
			}
			std::getline(inputStream, line);
			{
				std::istringstream ss(line);
				ss >> command >> a >> b >> c;
			}
			std::getline(inputStream, line);
			{
				std::istringstream ss(line);
				ss.ignore(sizeof("After:  [")-1);
				for (int i=0; i<4; i++) {
					ss >> after[i] >> shit;
				}
			}
			std::getline(inputStream, line);

			for (int i=0; i<16; i++) {

				auto temp = before;
				Execute((Command)i, temp, a, b, c);
				bool match = true;
				for (int i=0; i<4; i++) {
					if (temp[i] != after[i]) {
						match = false;
						break;
					}
				}
				if (match) {
					probs[command][(Command)i]++;
				}
			}
		}
		std::unordered_set<int> unresolved;
		std::unordered_set<Command> unmatched;
		for (int i=0; i<16; i++) {
			unresolved.emplace(i);
			unmatched.emplace((Command)i);
		}
		for (int i=0; i<16; i++) {
			for (auto& id : unresolved) {
				auto maxComm = (Command)0;
				int maxVal = 0;
				bool tie = false;
				for (auto& [comm, val] : probs[id]) {
					if (unmatched.count(comm) != 1) {
						continue;
					}
					if (maxVal < val) {
						maxVal = val;
						maxComm = comm;
						tie = false;
					}
					else if (val == maxVal) {
						tie = true;
					}
				}
				if (tie)
					continue;
				map[id] = maxComm;
				unresolved.erase(id);
				unmatched.erase(maxComm);
				break;
			}
		}
		std::vector<int> data(4,0);
		while (inputStream >> command >> a >> b >> c) {
			Execute(map[command], data, a, b, c);
		}
		inputStream.close();
		std::cout << data[0] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/