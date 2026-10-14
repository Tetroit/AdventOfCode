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
#include "vec.h"

//covering area with manhattan sensors

struct Task
{

	struct Diag {
		ivec2 o;
		ivec2 d;
		int l;
	};
	struct Manh {
		ivec2 o;
		int r;
	};

	static std::vector<Diag> extractDiagonals(Manh manh) {
		ivec2 bottom = manh.o - ivec2{0,manh.r + 1};
		ivec2 top = manh.o + ivec2{0,manh.r + 1};
		ivec2 left = manh.o - ivec2{manh.r + 1,0};
		ivec2 right = manh.o + ivec2{manh.r + 1,0};
		return std::vector<Diag>{
			Diag {top,{1, -1},manh.r},
			Diag {right, {-1, -1}, manh.r},
			Diag {bottom,{-1, 1},manh.r},
			Diag {left, {1, 1}, manh.r},
		};
	}
	static int floorDiv2(int x) {
		int q = x / 2;
		if (x % 2 != 0 && x < 0)
			q--;
		return q;
	}

	static bool splitByManh(Diag diag, const Manh& manh, std::vector<Diag>& out) {
		out.clear();

		int a = diag.d.x * (diag.o.x - manh.o.x);
		int b = diag.d.y * (diag.o.y - manh.o.y);

		if (std::abs(a - b) > manh.r) return false;

		int clipMin = -floorDiv2(manh.r + a + b);
		int clipMax = floorDiv2(manh.r - a - b);

		if (clipMax < 0 || clipMin > diag.l) return false;

		if (clipMin > 0) {
			out.emplace_back(diag.o, diag.d, clipMin - 1);
		}
		if (clipMax < diag.l) {
			out.emplace_back(diag.o + diag.d * (clipMax + 1), diag.d, diag.l - clipMax - 1);
		}
		return true;
	}

	static inline const std::regex signalPattern = std::regex(R"(Sensor at x=([-|\d]+), y=([-|\d]+): closest beacon is at x=([-|\d]+), y=([-|\d]+))",  std::regex::ECMAScript | std::regex::optimize);

	static inline std::unordered_set<ivec2, ivec2hash> beacons;
	static inline std::vector<Manh> sensorAreas;
	static void run() {

		std::ifstream inputStream("2022/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int checkY = 2000000;
		std::string line;
		std::vector<std::pair<int, int>> cover;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, signalPattern)) {
				ivec2 s = {std::stoi(match[1]), std::stoi(match[2])};
				ivec2 b = {std::stoi(match[3]), std::stoi(match[4])};
				beacons.insert(b);
				int r = (s - b).len();
				sensorAreas.emplace_back(s, r);
				int w = r - abs(s.y - checkY);
				if (w < 0) continue;
				int start = s.x - w;
				int end = s.x + w;
				cover.emplace_back(start, end);
			}
		}
		inputStream.close();
		if (cover.size() > 1) {
			std::sort(cover.begin(), cover.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				return a.first == b.first ? a.second < b.second : a.first < b.first;
			});
			for (int i=cover.size()-1; i>0; i--) {
				if (cover[i-1].second + 1 < cover[i].first) continue;
				bool consume = cover[i-1].second >= cover[i].second;
				if (!consume) {
					cover[i-1].second = cover[i].second;
				}
				cover.erase(cover.begin() + i);
				if (consume && i < cover.size())
					i++;
			}
		}
		int cnt = 0;
		for (auto& [start, end] : cover) {
			cnt += end - start + 1;
		}
		for (auto& b : beacons) {
			if (b.y == checkY)
				cnt--;
		}
		std::cout << cnt << std::endl;
		//6221085 high
	}
	static void runPart2() {

		int maxBound = 4000000;
		std::vector<Diag> diags;
		for (const auto& sensorArea : sensorAreas) {
			for (const auto& diag : extractDiagonals(sensorArea)) {
				diags.emplace_back(diag);
			}
		}
		for (const auto& sensorArea : sensorAreas) {
			for (int i=diags.size()-1; i>=0; i--) {
				std::vector<Diag> toAdd;
				if (splitByManh(diags[i], sensorArea, toAdd)) {
					diags.erase(diags.begin() + i);
					for (auto diag : toAdd) {
						diags.emplace_back(diag);
					}
				}
			}
		}
		for (auto diag : diags) {
			if (diag.o.x > 0 && diag.o.y > 0 && diag.o.x <= maxBound && diag.o.y <= maxBound) {
				// std::cout << diag.o.x << ", " << diag.o.y << ", " << diag.l << std::endl;
				std::cout << (long long)diag.o.x * 4000000 + diag.o.y << std::endl;
				break;
			}
		}
		std::cout << std::endl;
		//22412692282332 high
	}
};

//-------------- NOTES AREA ----------------
/*
	line vs manhattan
	r = |x - ox| + |y - oy|
	p{x,y} = s + a*n, where 0<=n<=l
	r = |sx + ax*n - ox| + |sy + ay*n - oy|
	using
	dx = sx - ox
	dy = sy - oy
	:
	r = sgnx*(rx + dx*n) + sgny*(ry + dy*n), with sgnx = {-1, 1} and sgny = {-1, 1}
	n = (r - sgnx*rx - sgny*ry) / (sgnx*dx + sgny*dy)

	masking:
	for n0 and n1 roots split original segment into 0<=n<n0 and n1<n<=l if possible

	but then there is division by 2...
	AUUGHHHHH

*/