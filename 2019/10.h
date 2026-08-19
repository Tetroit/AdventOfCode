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
#include "vec.h"
#include "utils.h"

//observing asteroids in straight line
struct Task
{
	static inline DynamicGrid<int> map;

	static inline ivec2 lazerPos = {0,0};
	static std::vector<ivec2> directions(int width, int height, int x, int y) {
		std::vector<ivec2> range{{0, -1}, {1, 0}};
		generateBetween(width, height, x, y, range, range.size() - 2);
		range.emplace_back(0,1);
		generateBetween(width, height, x, y, range, range.size() - 2);
		range.emplace_back(-1,0);
		generateBetween(width, height, x, y, range, range.size() - 2);
		range.emplace_back(0,-1);
		generateBetween(width, height, x, y, range, range.size() - 2);
		range.pop_back();
		return range;
	}
	//using Stern–Brocot tree
	static int generateBetween(int width, int height, int x, int y, std::vector<ivec2>& arr, int start = 0) {
		ivec2 left = arr[start];
		ivec2 right = arr[start + 1];
		ivec2 middle = left + right;
		int shootX = x+middle.x;
		int shootY = y+middle.y;
		if (shootX < 0 || shootY < 0 || shootX >= width || shootY >= height) {
			return 0;
		}
		arr.insert(arr.begin()+start+1, middle);
		int cnt = 1;
		cnt += generateBetween(width, height, x, y, arr, start);
		cnt += generateBetween(width, height, x, y, arr, start + cnt);
		return cnt;
	}
	static void run() {

		std::ifstream inputStream("2019/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int inY = 0;
		while (std::getline(inputStream, line))
		{
			map.fillFromLine(inY++, line, [](char val){return val == '#' ? 1 : 0;});
		}
		inputStream.close();

		std::vector<ivec2> asteroids;
		std::unordered_map<ivec2, int, ivec2hash> cnt;
		map.foreachCoord([&asteroids](int x, int y){if (map.get(x, y) == 1) asteroids.emplace_back(x, y);});
		for (int i = 0; i < (int)asteroids.size()-1; ++i) {
			ivec2& a = asteroids[i];
			for (int j = i+1; j < (int)asteroids.size(); ++j) {
				ivec2& b = asteroids[j];
				auto diff = (a - b);
				auto diffAbs = diff.abs();
				int div = Utils::gcd(diffAbs.x, diffAbs.y);
				if (div == 1) {
					cnt[a]++;
					cnt[b]++;
					continue;
				}
				auto step = diff/div;
				auto ptr = b + step;
				bool obstructed = false;
				while (ptr != a) {
					if (map.get(ptr.x, ptr.y) == 1) {
						obstructed = true;
						break;
					}
					ptr += step;
				}
				if (!obstructed) {
					cnt[a]++;
					cnt[b]++;
					continue;
				}
			}
		}
		int maxCnt = 0;
		for (auto& [pos, val] : cnt) {
			if (val > maxCnt) {
				maxCnt = val;
				lazerPos = pos;
			}
		}
		std::cout << maxCnt << std::endl;
	}
	static void runPart2() {
		auto shootingSeq = directions(map.getWidth(), map.getHeight(), lazerPos.x, lazerPos.y);
		int destroyed = 0;
		ivec2 lastDestroyed;
		int dirId = 0;
		while (destroyed < 200) {
			auto dir = shootingSeq[dirId++ % shootingSeq.size()];
			ivec2 ray = lazerPos+dir;
			while (ray.x >= 0 && ray.y >= 0 && ray.x < map.getWidth() && ray.y < map.getHeight()) {
				if (map.get(ray.x, ray.y) == 1) {
					map.set(ray.x, ray.y, 0);
					destroyed++;
					lastDestroyed = ray;
					break;
				}
				ray+=dir;
			}
		}
		std::cout << lastDestroyed.x * 100 + lastDestroyed.y << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/