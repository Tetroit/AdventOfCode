#pragma once

#include <bitset>
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
#include "vec.h"

//Fancy convolution stuff
struct Task
{
	static inline std::bitset<512> function;
	static inline std::unordered_map<ivec2, bool, ivec2hash> initImage;

	static void expand(std::unordered_map<ivec2, bool, ivec2hash>& image, const bool background) {
		std::vector<ivec2> init;
		for (auto [pos, val] : image) {
			init.push_back(pos);
		}
		for (auto pos : init) {
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0) continue;
					auto neighbour = pos + ivec2{dx,dy};
					if (!image.contains(neighbour)) image[neighbour] = background;
				}
			}
		}
	}

	static uint16_t getCode(const std::unordered_map<ivec2, bool, ivec2hash>& image, ivec2 pos, const bool background) {
		uint16_t res = 0;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				res <<= 1;
				ivec2 src = pos + ivec2{dx,dy};
				res |= image.contains(src) ? image.at(src) : background;
			}
		}
		return res;
	}

	static void print (const std::unordered_map<ivec2, bool, ivec2hash>& image, const bool background) {
		ivec2 min = ivec2{INT_MAX, INT_MAX};
		ivec2 max = ivec2{INT_MIN, INT_MIN};
		for (auto [pos, val] : image) {
			if (pos.x < min.x) min.x = pos.x;
			if (pos.y < min.y) min.y = pos.y;
			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;
		}
		for (int x = min.x; x <= max.x; x++) {
			for (int y = min.y; y <= max.y; y++) {
				if (!image.contains(ivec2{x,y})) std::cout << (background ? "#" : ".");
				else std::cout << (image.at(ivec2{x,y}) ? '#' : '.');
			}
			std::cout << '\n';
		}
		std::cout << std::endl;
	}
	static void run() {

		std::ifstream inputStream("2021/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::getline(inputStream, line);
		for (int i = 0; i < line.size(); i++) {
			function[i] = line[i] == '#' ? 1 : 0;
		}
		std::getline(inputStream, line);
		int rY = 0;
		while (std::getline(inputStream, line))
		{
			for (int i=0; i<line.size(); i++) {
				if (line[i] == '#')
					initImage[ivec2{rY,i}] = true;
			}
			rY++;
		}
		inputStream.close();

		auto img1 = initImage;
		auto img2 = initImage;
		auto* src = &img1;
		auto* dst = &img2;
		bool background = false;
		bool newBackground = false;

		static constexpr int repeats = 2;
		for (int i=0; i<repeats; i++) {
			newBackground = background ? function[511] : function[0];
			dst->clear();
			expand(*src, background);
			for (const auto& [pos, val] : *src) {
				auto code = getCode(*src, pos, background);
				bool res = function[code];
				if (res != newBackground)
					dst->operator[](pos) = !newBackground;
			}
			std::swap(src, dst);
			background = newBackground;
			// print(*src, background);
		}
		std::cout << src->size() << std::endl;
	}
	static void runPart2() {
		auto img1 = initImage;
		auto img2 = initImage;
		auto* src = &img1;
		auto* dst = &img2;
		bool background = false;
		bool newBackground = false;

		static constexpr int repeats2 = 50;
		for (int i=0; i<repeats2; i++) {
			newBackground = background ? function[511] : function[0];
			dst->clear();
			expand(*src, background);
			for (const auto& [pos, val] : *src) {
				auto code = getCode(*src, pos, background);
				bool res = function[code];
				if (res != newBackground)
					dst->operator[](pos) = !newBackground;
			}
			std::swap(src, dst);
			background = newBackground;
			// print(*src, background);
		}
		std::cout << src->size() << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/