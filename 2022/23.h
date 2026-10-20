#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//Spreading elves automata

struct Task
{
	static inline const std::array<ivec2, 8> round = std::array<ivec2, 8>{
			{
				{-1,-1},
				{0,-1,},
				{1,-1},
				{1,0},
				{1,1},
				{0,1},
				{-1,1},
				{-1,0}
			}};
	static inline const std::array<ivec2, 4> facing = std::array<ivec2, 4>{{
		{1,0},{0,1},{-1, 0}, {0,-1}
	}};
	struct Elf {
		explicit Elf(ivec2 pos) : pos(pos), proposedDir(-1) {
			checkQueue = {3,1,2,0};
		}
		ivec2 pos;
		std::vector<int> checkQueue;
		int proposedDir = -1;
		bool canMove(int dir, const std::unordered_map<ivec2, Elf, ivec2hash>& m) const {
			switch (dir) {
				case 0: return (!m.contains(pos + round[2]) && !m.contains(pos + round[3]) && !m.contains(pos + round[4]));
				case 1: return (!m.contains(pos + round[4]) && !m.contains(pos + round[5]) && !m.contains(pos + round[6]));
				case 2: return (!m.contains(pos + round[6]) && !m.contains(pos + round[7]) && !m.contains(pos + round[0]));
				case 3: return (!m.contains(pos + round[0]) && !m.contains(pos + round[1]) && !m.contains(pos + round[2]));
				default: return false;
			}
		}
		void selectDir(const std::unordered_map<ivec2, Elf, ivec2hash>& m) {
			int neighbours = 0;
			for (auto f : round) {
				if (m.contains(pos + f)) neighbours++;
			}
			if (neighbours == 0) return;
			for (int i=0; i<4; i++) {
				int rot = checkQueue[i];
				if (canMove(rot, m)) {
					proposedDir = rot;
					break;
				}
			}
		}
		ivec2 getNextPos() const {
			if (proposedDir >= 0 && proposedDir < 4) return facing[proposedDir] + pos;
			return pos;
		}
		void moveInSelected() {
			int startFacing = checkQueue[0];
			checkQueue.erase(checkQueue.begin());
			checkQueue.emplace_back(startFacing);
			pos = getNextPos();
			proposedDir = -1;
		}
	};
	static inline std::unordered_map<ivec2, Elf, ivec2hash> map;
	static void run() {

		std::ifstream inputStream("2022/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int y = 0;
		while (std::getline(inputStream, line))
		{
			for (int x=0; x<line.size(); x++) {
				if (line[x] == '#') {
					map.emplace(ivec2(x, y), Elf(ivec2(x, y)));
				}
			}
			y++;
		}
		inputStream.close();
		std::unordered_map<ivec2, Elf, ivec2hash> buff1 = map;
		std::unordered_map<ivec2, Elf, ivec2hash> buff2 = buff1;
		auto src = &buff1;
		auto dst = &buff2;
		for (int rep = 0; rep < 10; rep++) {

			dst->clear();
			std::unordered_map<ivec2, std::vector<Elf*>, ivec2hash> proposed;
			for (auto& [pos, elf] : *src) {
				elf.selectDir(*src);
				proposed[elf.getNextPos()].emplace_back(&elf);
			}
			for (auto &candList: proposed | std::views::values) {
				if (candList.size() > 1) {
					for (auto candidate : candList) {
						candidate->proposedDir = -1;
					}
				}
			}
			for (auto &elf: *src | std::views::values) {
				elf.moveInSelected();
				dst->emplace(elf.pos, elf);
			}
			std::swap(src, dst);

			// DynamicGrid<char> res;
			// res.fillFromCoordList(*src | std::views::keys, '#', '.', [](ivec2 vec){return std::make_pair(vec.x, vec.y);});
			// res.print(DefaultConvert<char>());
			// std::cout << std::endl;
		}
		ivec2 min = {INT_MAX, INT_MAX};
		ivec2 max = {INT_MIN, INT_MIN};
		for (auto& pos : *src | std::views::keys) {
			if (min.x > pos.x) min.x = pos.x;
			if (min.y > pos.y) min.y = pos.y;
			if (max.x < pos.x) max.x = pos.x;
			if (max.y < pos.y) max.y = pos.y;
		}
		std::cout << (max.x - min.x + 1) * (max.y - min.y + 1) - src->size() << std::endl;

	}
	static void runPart2() {
		std::unordered_map<ivec2, Elf, ivec2hash> buff1 = map;
		std::unordered_map<ivec2, Elf, ivec2hash> buff2 = buff1;
		auto src = &buff1;
		auto dst = &buff2;
		bool changed = false;
		int moves = 0;
		do {
			moves++;
			changed = false;
			dst->clear();
			std::unordered_map<ivec2, std::vector<Elf*>, ivec2hash> proposed;
			for (auto& [pos, elf] : *src) {
				elf.selectDir(*src);
				if (elf.proposedDir != -1) changed = true;
				proposed[elf.getNextPos()].emplace_back(&elf);
			}
			for (auto &candList: proposed | std::views::values) {
				if (candList.size() > 1) {
					for (auto candidate : candList) {
						candidate->proposedDir = -1;
					}
				}
			}
			for (auto &elf: *src | std::views::values) {
				elf.moveInSelected();
				dst->emplace(elf.pos, elf);
			}
			std::swap(src, dst);

			// DynamicGrid<char> res;
			// res.fillFromCoordList(*src | std::views::keys, '#', '.', [](ivec2 vec){return std::make_pair(vec.x, vec.y);});
			// res.print(DefaultConvert<char>());
			// std::cout << std::endl;
		} while (changed);
		ivec2 min = {INT_MAX, INT_MAX};
		ivec2 max = {INT_MIN, INT_MIN};
		for (auto& pos : *src | std::views::keys) {
			if (min.x > pos.x) min.x = pos.x;
			if (min.y > pos.y) min.y = pos.y;
			if (max.x < pos.x) max.x = pos.x;
			if (max.y < pos.y) max.y = pos.y;
		}
		std::cout << moves << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/