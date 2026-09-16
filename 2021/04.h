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

#include "GridBase.h"
#include "utils.h"

//playing bingo

struct Task
{
	using Bingo = Grid<5,5,int>;
	using BingoMask = Grid<5,5,bool>;

	static inline std::vector<int> seq;
	static inline std::vector<Bingo> bingos;
	static inline std::vector<BingoMask> bingoMasks;

	static bool checkBingo(BingoMask& bingo, Bingo& bingoNums) {
		for (int x = 0; x < bingo.getWidth(); x++) {
			bool filled = true;
			for (int y = 0; y < bingo.getHeight(); y++) {
				if (!bingo.get(x,y)) {
					filled = false;
					break;
				}
			}
			if (filled) return true;
		}
		for (int y = 0; y < bingo.getHeight(); y++) {
			bool filled = true;
			for (int x = 0; x < bingo.getWidth(); x++) {
				if (!bingo.get(x,y)) {
					filled = false;
					break;
				}
			}
			if (filled) return true;
		}
		return false;
	}
	static void run() {

		std::ifstream inputStream("2021/04.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		auto words = Utils::split(line, ',');
		for (auto& word : words) {
			seq.push_back(std::stoi(word));
		}

		int readY = 0;
		while (std::getline(inputStream, line))
		{
			if (line == "") {
				readY = 0;
				bingos.emplace_back();
				bingoMasks.emplace_back();
				continue;
			}
			words = Utils::split(line, ' ');
			for (int i=words.size()-1; i>=0; i--) {
				if (words[i].empty()) words.erase(words.begin()+i);
			}
			for (int i=0; i<5; i++) {
				bingos.back().set(i, readY, std::stoi(words[i]));
			}
			readY++;
		}
		inputStream.close();

		int winId = -1;
		int winNum = -1;

		for (auto& num : seq) {
			for (int i=0; i<bingos.size(); i++) {
				auto& bingo = bingos[i];
				auto& mask = bingoMasks[i];
				bingo.foreachCoord([&](int x, int y) {
					if (bingo.get(x, y) == num) {
						mask.set(x, y, true);
					}
				});
				if (checkBingo(mask, bingo)) {
					winId = i;
					break;
				}
			}
			if (winId != -1) {
				winNum = num;
				break;
			}
		}

		int uncircled = 0;
		bingos[winId].foreachCoord([&](int x, int y) {
			if (!bingoMasks[winId].get(x, y)) {
				uncircled += bingos[winId].get(x, y);
			}
		});
		std::cout << winNum << ' ' << uncircled << ' ' << winNum * uncircled << std::endl;
	}
	static void runPart2() {
		for (auto& bingoMask : bingoMasks) {
			bingoMask.clear(false);
		}
		int lastId = -1;
		int winNum = -1;
		for (auto& num : seq) {
			bool allBingos = true;
			for (int i=0; i<bingos.size(); i++) {
				auto& bingo = bingos[i];
				auto& mask = bingoMasks[i];
				bingo.foreachCoord([&](int x, int y) {
					if (bingo.get(x, y) == num) {
						mask.set(x, y, true);
					}
				});
				if (!checkBingo(mask, bingo)) {
					allBingos = false;
					lastId = i;
				}
			}
			if (allBingos) {
				winNum = num;
				break;
			}
		}

		int uncircled = 0;
		bingos[lastId].foreachCoord([&](int x, int y) {
			if (!bingoMasks[lastId].get(x, y)) {
				uncircled += bingos[lastId].get(x, y);
			}
		});
		std::cout << winNum << ' ' << uncircled << ' ' << winNum * uncircled << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/