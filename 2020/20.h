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
#include <__msvc_ranges_to.hpp>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"
//combining image from pieces

struct Task
{

	static inline constexpr int fragmentSize = 10;
	static std::array<uint16_t, 8> readSides(const Grid<fragmentSize, fragmentSize, int>& fragment) {
		std::array<uint16_t, 8> result{};
		for (int i=0; i < fragmentSize; i++) {
			int inv = fragmentSize - i - 1;
			result[0] |= (fragment.get(fragmentSize - 1, inv) << i);
			result[1] |= (fragment.get(inv, 0) << i);
			result[2] |= (fragment.get(0, i) << i);
			result[3] |= (fragment.get(i, fragmentSize - 1) << i);

			result[4] |= (fragment.get(fragmentSize - 1, inv) << inv);
			result[5] |= (fragment.get(inv, 0) << inv);
			result[6] |= (fragment.get(0, i) << inv);
			result[7] |= (fragment.get(i, fragmentSize - 1) << inv);
		}
		return result;

	}

	//seems like this is useless and input actually prevents collisions
	static std::optional<std::array<int, 4>> canArrange(std::array<std::vector<int>, 4>& candidates) {
		std::array<int, 4> result{};
		std::array<bool, 4> locked{false, false, false, false};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (locked[j]) continue;
				if (candidates[j].empty()) return std::nullopt;
				if (candidates[j].size() == 1) {
					result[j] = candidates[j][0];
					locked[j] = true;
					for (int k = 0; k < 4; k++) {
						if (k==j) continue;
						auto duplicateLoc = std::find(candidates[k].begin(), candidates[k].end(), result[j]);
						if (duplicateLoc != candidates[k].end()) {
							candidates[k].erase(duplicateLoc);
						}
					}
				}
			}
		}
		return result;
	}

	static inline std::vector<Grid<fragmentSize, fragmentSize, int>> fragments;
	static inline std::vector<int> ids;
	static inline std::vector<std::array<uint16_t, 8>> sides;
	static void run() {

		std::ifstream inputStream("2020/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;

		while (std::getline(inputStream, line))
		{
			fragments.emplace_back();
			Grid<fragmentSize, fragmentSize, int>& fragment = fragments.back();
			ids.push_back(std::stoi(line.substr(5, 4)));
			for (int i=0; i < fragmentSize; i++) {
				std::getline(inputStream, line);
				fragment.fillFromLine(i, line, [](char ch){return ch == '#' ? 1 : 0;} );
			}
			std::getline(inputStream, line);
		}
		inputStream.close();
		for (const auto& fragment : fragments) {
			sides.push_back(readSides(fragment));
		}

		unsigned long long mult = 1;
		for (int fa=0; fa<fragments.size(); fa++) {
			const auto& fragmentA = fragments[fa];
			const auto& sidesA = sides[fa];
			std::array<std::vector<int>, 4> neighbours{};
			for (int fb=0; fb<fragments.size(); fb++) {
				if (fb == fa) continue;
				const auto& fragmentB = fragments[fb];
				const auto& sidesB = sides[fb];
				int matchedSides = 0;
				for (int sa = 0; sa < 4; sa++) {
					for (int sb = 0; sb < 8; sb++) {
						if (sidesA[sa] == sidesB[sb]) {
							neighbours[sa].push_back(fb);
							break;
						}
					}
				}
			}
			std::cout << neighbours[0].size() << ' ' << neighbours[1].size() << ' ' << neighbours[2].size() << ' ' << neighbours[3].size() << std::endl;
			// auto res = canArrange(candidates);

			int emptySides = 0;
			for (auto& sideCandidates : neighbours) {
				if (sideCandidates.empty()) emptySides++;
			}
			if (emptySides == 2) {
				mult *= ids[fa];
				std::cout << fa << std::endl;
			}
		}
		std::cout << mult << std::endl;
		//8523152029630028453 high
	}

	//some group theory stuff (group D4)

	static int transformSide(int transform, int side) {
		side = ((transform & 0b11) + side) & 0b11;
		if (transform >> 2 & 1) side = (4-side) & 0b11;
		return side;
	}
	static void transformPos(int& x, int& y, int size, int transform) {
		int flip = transform >> 2 & 1;
		int rotation = transform & 0b11;
		for (int i=0; i < rotation; i++) {
			int temp = x;
			x = y;
			y = size - 1 - temp;
		}
		if (flip) y = size-y-1;
	}
	static int oppositeOrientation(int a) {
		int rotA = a & 0b11;
		int flipA = a >> 2 & 1;
		if (!flipA) rotA = (4-rotA) & 0b11;
		return flipA << 2 | rotA;
	}
	static int calculateOrientation(int local, int world, bool flip) {
		if (flip) world = - world;
		return flip << 2 | Utils::Loop(world - local, 0, 3);
	}
	static void runPart2() {
		DynamicGrid<std::pair<int, int>> layout;
		using orientation = int;
		std::vector<std::array<std::optional<std::pair<int, orientation>>, 4>> neighbours(fragments.size());

		bool hasCorner = false;
		for (int fa=0; fa<fragments.size(); fa++) {
			const auto& fragmentA = fragments[fa];
			const auto& sidesA = sides[fa];
			for (int fb=0; fb<fragments.size(); fb++) {
				if (fb == fa) continue;
				const auto& fragmentB = fragments[fb];
				const auto& sidesB = sides[fb];
				int matchedSides = 0;
				for (int sa = 0; sa < 4; sa++) {
					for (int sb = 0; sb < 8; sb++) {
						if (sidesA[sa] == sidesB[sb]) {
							neighbours[fa][sa] = {fb, Utils::Loop(sb - 4, 0,7)};
							break;
						}
					}
				}
			}

			int emptySides = 0;
			std::array<bool, 4> isSideEmpty{};
			for (int sa = 0; sa < 4; sa++) {
				auto& sideCandidates = neighbours[fa][sa];
				if (!sideCandidates.has_value()) {
					emptySides++;
					isSideEmpty[sa] = true;
				}
			}

			if (emptySides == 2) {
				if (!hasCorner) {
					int startOrientation;
					hasCorner = true;
					if (isSideEmpty[0] && isSideEmpty[1]) {startOrientation = 1;}
					if (isSideEmpty[1] && isSideEmpty[2]) {startOrientation = 0;}
					if (isSideEmpty[2] && isSideEmpty[3]) {startOrientation = 3;}
					if (isSideEmpty[3] && isSideEmpty[0]) {startOrientation = 2;}
					layout.add(0,0, {fa,startOrientation});
				}
			}
		}
		auto [previousIdY, previousOrientationY] = layout.get(0, 0);
		for (int y=0; y<neighbours.size(); y++) {
			orientation previousOrientationX = previousOrientationY;
			int previousIdX = previousIdY;
			for (int x=1; x<neighbours.size(); x++) {
				int inverseSide = transformSide(oppositeOrientation(previousOrientationX), 0);
				auto next = neighbours[previousIdX][inverseSide];
				if (!next.has_value()) break;
				auto [fb, sb] = next.value();
				bool flipNew = ((previousOrientationX >> 2) & 1) ^ ((sb >> 2) & 1);
				previousOrientationX = calculateOrientation(sb & 0b11, 2, flipNew);
				previousIdX = fb;
				layout.add(x, y, {fb,  previousOrientationX});
			}
			int inverseSide = transformSide(oppositeOrientation(previousOrientationY), 3);
			auto next = neighbours[previousIdY][inverseSide];
			if (!next.has_value()) break;
			auto [fb, sb] = next.value();
			bool flipNew = ((previousOrientationY >> 2) & 1) ^ ((sb >> 2) & 1);
			previousOrientationY = calculateOrientation(sb & 0b11, 1, flipNew);
			previousIdY = fb;
			layout.add(0, y+1, {fb,  previousOrientationY});

			auto test = transformSide(oppositeOrientation(previousOrientationY), 2);
			std::cout << neighbours[previousIdY][test].has_value() << std::endl;
		}
		for (int i=0; i<layout.getHeight(); i++) {
			for (int j=0; j<layout.getWidth(); j++) {
				std::cout << ids[layout.get(j, i).first] << " ";
			}
			std::cout << std::endl;
		}
		DynamicGrid<uint8_t> canvas;
		canvas.resize(layout.getWidth() * (fragmentSize-2), layout.getHeight() * (fragmentSize-2));
		for (int y = 0; y<layout.getHeight(); y++) {
			for (int x = 0; x<layout.getWidth(); x++) {
				auto [fragID, transform] = layout.get(x, y);

				const auto& fragment = fragments[fragID];
				for (int xl = 1; xl<fragmentSize-1; xl++) {
					for (int yl = 1; yl<fragmentSize-1; yl++) {

						int xw = xl;
						int yw = yl;
						transformPos(xw, yw, fragmentSize, transform);
						xw += (fragmentSize-2) * x - 1;
						yw += (fragmentSize-2) * y - 1;
						canvas.set(xw, yw, fragment.get(xl, yl));
					}
				}
			}
		}
		canvas.print([](uint8_t val){return val == 1 ? '#' : '.';});


		Grid<20, 3, uint8_t> snakeSchematic;
		std::string snakeStr =
			"                  # \n"
			"#    ##    ##    ###\n"
			" #  #  #  #  #  #   ";
		std::istringstream snakeStream(snakeStr);
		snakeSchematic.fillFromStream(snakeStream, [](char ch) ->uint8_t {return ch == '#' ? 1 : 0;});
		for (int tr = 0; tr < 8; tr++) {
			int cnt = 0;
			std::unordered_set<ivec2, ivec2hash> allSnakePos;
			for (int oy=0; oy<canvas.getHeight() - snakeSchematic.getHeight(); oy++) {
				for (int ox=0; ox<canvas.getWidth() - snakeSchematic.getWidth(); ox++) {
					bool match = true;
					std::unordered_set<ivec2, ivec2hash> snakePos;
					for (int xl = 0; xl < snakeSchematic.getWidth(); xl++) {
						for (int yl = 0; yl < snakeSchematic.getHeight(); yl++) {
							int xw = xl;
							int yw = yl;
							xw+=ox;
							yw+=oy;
							transformPos(xw, yw, canvas.getHeight(), tr);
							if (canvas.get(xw, yw) == 0 && snakeSchematic.get(xl, yl) == 1) {
								match = false;
								break;
							}
							if (snakeSchematic.get(xl, yl) == 1) {
								snakePos.emplace(xw, yw);
							}
						}
						if (!match) break;
					}
					if (match) {
						for (auto pos : snakePos) {
							allSnakePos.emplace(pos);
						}
						cnt++;
					}
				}
			}
			if (cnt != 0) {
				int snakeCnt = allSnakePos.size();
				int total = canvas.count([](uint8_t val){return val == 1;});
				std::cout << total - snakeCnt << std::endl;
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

	each square can connect to other squares in 8 possible ways
	which can be determined by reading an edge in one way or opposite
	reading values in directions 0-3 correspond to the original orientation
	values 4-7 represent sides when flipped

	there exist in total 8 different solutions of this puzzle which is technically the same solution but rotated and/or flipped
	so we can only care about matching original orientation of 1 of the pieces to every other one

	*<1---------------5>*
	^                   ^
	6                   0
	|                   |
	|                   |
	|                   |
	|                   |
	2                   4
	v                   v
	*<7---------------3>*

	transform: rotate THEN flip around X axis

	THIS IS HORRIBLE ABSOLUTELY HORRIBLE
	THE GOD DAMN ORIENTATIONS
	MAKING GENERAL CONVENTION IS ALREADY A PROBLEM... WHAT ABOUT APPLICATIONS??
	READING ORDER THEN THIS ONE ROTATED, THIS ONE FLIPPED AAAAAAAAAAAAAAAAAAA
	i did my best using D4 group but gl with the rest

*/