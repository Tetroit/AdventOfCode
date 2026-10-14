#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"
#include "vec.h"

//Minecraft simulator, counting cube sides
struct Task
{
	static inline const std::vector<ivec3> normals{
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1}
	};
	static inline std::unordered_set<ivec3, ivec3hash> positions;
	static void run() {

		std::ifstream inputStream("2022/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		ivec3 pos;
		char shit;
		while (inputStream >> pos.x >> shit >> pos.y >> shit >> pos.z)
		{
			positions.insert(pos);
		}
		inputStream.close();
		int cnt = 0;
		for (auto it = positions.begin(); it != positions.end(); ++it) {
			ivec3 curr = *it;
			int sides = 6;
			for (const auto& normal : normals) {
				if (positions.contains(curr + normal)) sides--;
			}
			cnt += sides;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::vector<int> map;
		ivec3 min = {INT_MAX, INT_MAX, INT_MAX};
		ivec3 max = {INT_MIN, INT_MIN, INT_MIN};
		for (const auto& pos : positions) {
			if (pos.x < min.x) min.x = pos.x;
			if (pos.y < min.y) min.y = pos.y;
			if (pos.z < min.z) min.z = pos.z;

			if (pos.x > max.x) max.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;
			if (pos.z > max.z) max.z = pos.z;
		}
		ivec3 size = max - min + ivec3{1,1,1};
		map.resize(size.x * size.y * size.z);

		std::vector<int> bubbleId;
		bubbleId.resize(size.x * size.y * size.z);

		auto getMapCell = [&map, min, size](ivec3 pos) -> int& {

		};

		for (auto& pos : positions) {
			auto lPos = pos - min;
			int key = lPos.x * size.y * size.z + lPos.y * size.z + lPos.z;
			map[key] = 1;
		}

		int nextBubble = 1;
		std::unordered_set<int> closedBubbles;
		for (int x=0; x<size.x; x++) {
			for (int y=0; y<size.y; y++) {
				for (int z=0; z<size.z; z++) {
					if (bubbleId[(x*size.y + y)*size.z + z] == 0) {
						bool open = false;
						std::deque<ivec3> q {{x,y,z}};
						while (!q.empty()) {
							auto el = q.front();
							bubbleId[(el.x*size.y + el.y)*size.z + el.z] = nextBubble;
							q.pop_front();
							for (const auto& normal : normals) {
								auto nEl = normal + el;
								if (nEl.x < 0 || nEl.y < 0 || nEl.z < 0 || nEl.x >= size.x || nEl.y >= size.y || nEl.z >= size.z) {
									open = true;
									continue;
								}
								int cell = (nEl.x*size.y + nEl.y) * size.z + nEl.z;
								if (map[cell] == 0 &&
									bubbleId[cell] == 0) {
									bool queued = false;
									for (auto& qEl : q) {
										if (qEl == nEl) {
											queued = true;
											break;
										}
									}
									if (!queued)
										q.push_back(nEl);
								}
							}
						}
						if (!open) closedBubbles.insert(nextBubble);
						nextBubble++;
					}
				}
			}
		}
		for (int x=0; x<size.x; x++) {
			for (int y=0; y<size.y; y++) {
				for (int z=0; z<size.z; z++) {
					if (map[(x*size.y + y)*size.z + z] != 0) continue;
					if (closedBubbles.contains(bubbleId[(x*size.y + y)*size.z + z])) {
						map[(x*size.y + y)*size.z + z] = 2;
					}
				}
			}
		}

		int cnt = 0;
		for (const auto& pos : positions) {
			int faces = 6;
			auto lPos = pos - min;
			for (const auto& normal : normals) {
				auto n = lPos + normal;
				if (n.x < 0 || n.y < 0 || n.z < 0 || n.x >= size.x || n.y >= size.y || n.z >= size.z) continue;
				if (map[(n.x * size.y + n.y) * size.z + n.z] != 0) faces--;
			}
			cnt += faces;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/