#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
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

// follow instructions with warping around edges on a cube
struct Task
{
	static inline const std::vector<ivec2> facingVec{{1,0}, {0,1}, {-1,0}, {0,-1}};
	static constexpr std::array<std::array<int, 3>,8> cubeVerts{{
		{{1,3,4}},
		{{0,2,5}},
		{{1,3,6}},
		{{0,2,7}},
		{{0,5,7}},
		{{1,4,6}},
		{{2,5,7}},
		{{3,4,6}}
	}};
	static inline std::string pathStr;
	static inline DynamicGrid<uint8_t> map;
	static inline DynamicGrid<uint8_t> cubeMask;

	static int captureInstruction(const std::string& line, int& ptr) {
		if (std::isupper(line[ptr])) {
			return line[ptr++] == 'R' ? -1 : -2;
		}
		int steps = 0;
		while (ptr < line.size() && !std::isupper(line[ptr])) {
			steps = steps * 10 + line[ptr++] - '0';
		}
		return steps;
	}
	static std::unordered_map<ivec3, ivec3, ivec3hash> warpMap(int size) {

		std::unordered_map<ivec3, ivec3, ivec3hash> res;

		auto thirdVert = [](int p, int a, int b) {
			auto& verts = cubeVerts.at(p);
			if (verts[0] != a && verts[0] != b) return verts[0];
			if (verts[1] != a && verts[1] != b) return verts[1];
			if (verts[2] != a && verts[2] != b) return verts[2];
			return -1;
		};
		auto oppositeCorner = [](ivec2 cell, ivec2 corner) {
			return cell + ivec2{1, 1} - (corner - cell);
		};
		auto facingOf = [&](ivec2 dir) {
			return dir.x == 0 ? (dir.y > 0 ? 1 : 3) : (dir.x > 0 ? 0 : 2);
		};

		cubeMask.resize(map.getWidth()/size, map.getHeight()/size);
		cubeMask.foreachCoord([&](int x, int y) {
			cubeMask.set(x,y,map.get(x*size, y*size) == 0 ? 0 : 1);
		});
		DynamicGrid<int8_t> vertMap;
		vertMap.resize(cubeMask.getWidth() + 1, cubeMask.getHeight() + 1);
		vertMap.clear(-1);

		auto vertAt = [&](ivec2 pos) {
			return vertMap.get(pos.x, pos.y);
		};

		//start with 1 face and DFS to spread vertex IDs

		std::deque<ivec2> q;
		std::unordered_set<ivec2, ivec2hash> visited;
		for (int x = 0; x<cubeMask.getWidth(); x++) {
			if (cubeMask.get(x,0)) {
				q.emplace_back(x,0);
				visited.insert({x,0});
				vertMap.set(x,0,0);
				vertMap.set(x+1,0,1);
				vertMap.set(x+1,1,2);
				vertMap.set(x,1,3);
				break;
			}
		}
		// cubeMask.print([](uint8_t val) {return val ? '#' : '.';});
		// std::cout << std::endl;

		//also record which edges will lead us outwards, we will stitch them later
		std::vector<std::pair<ivec2, ivec2>> outEdges;
		while (!q.empty()) {
			ivec2 face = q.front();
			q.pop_front();
			for (auto d : facingVec) {
				auto nFace = face + d;

				if (visited.contains(nFace)) continue;

				ivec2 dv1, dv2;
				//this condition will keep the 2 vertices always sorted
				if (d.x != 0) {
					dv1 = {d.x > 0 ? 1 : 0, 0};
					dv2 = {d.x > 0 ? 1 : 0, 1};
				} else {
					dv1 = {0, d.y > 0 ? 1 : 0};
					dv2 = {1, d.y > 0 ? 1 : 0};
				}
				ivec2 f1 = face + dv1;
				ivec2 f2 = face + dv2;

				if (!cubeMask.inRange(nFace.x, nFace.y) || !cubeMask.get(nFace.x,nFace.y)) {
					if (vertMap.get(f1.x, f1.y) < vertMap.get(f2.x, f2.y))
						outEdges.emplace_back(f1, f2);
					else
						outEdges.emplace_back(f2, f1);
					// std::cout << "Pushed edge " << (int)vertAt(f1) << " " << (int)vertAt(f2) << std::endl;
					continue;
				}

				ivec2 b1 = oppositeCorner(face, f2);
				ivec2 b2 = oppositeCorner(face, f1);
				ivec2 o1 = oppositeCorner(nFace, f2);
				ivec2 o2 = oppositeCorner(nFace, f1);
				vertMap.set(o1.x, o1.y, thirdVert(vertMap.get(f1.x, f1.y), vertMap.get(f2.x, f2.y), vertMap.get(b1.x, b1.y)));
				vertMap.set(o2.x, o2.y, thirdVert(vertMap.get(f2.x, f2.y), vertMap.get(f1.x, f1.y), vertMap.get(b2.x, b2.y)));

				visited.insert(nFace);
				q.push_back(nFace);
			}
			// vertMap.print([](int8_t val) {return val == -1 ? '.' : (char)(val + '0');});
			// std::cout << std::endl;
		}

		//match all edges (already sorted)
		while (!outEdges.empty()) {
			auto& [a1, a2] = outEdges.front();
			for (int i = 1; i < outEdges.size(); i++) {
				auto& [b1, b2] = outEdges[i];
				if (vertAt(a1) == vertAt(b1) && vertAt(a2) == vertAt(b2) ) {
					ivec2 dA = a2 - a1;
					ivec2 dB = b2 - b1;

					//normals are facing to where there is an empty vertex in the grid
					ivec2 nA, nB;
					if (dA.x == 0) {
						nA = {-1, 0};
						if (a1.x != 0 && vertAt(a1 + nA) != -1 && vertAt(a2 + nA) != -1)
							nA = {1, 0};
					}
					else {
						nA = {0, -1};
						if (a1.y != 0 && vertAt(a1 + nA) != -1 && vertAt(a2 + nA) != -1)
							nA = {0, 1};
					}

					if (dB.x == 0) {
						nB = {-1, 0};
						if (b1.x != 0 && vertAt(b1 + nB) != -1 && vertAt(b2 + nB) != -1)
							nB = {1, 0};
					}
					else {
						nB = {0, -1};
						if (b1.y != 0 && vertAt(b1 + nB) != -1 && vertAt(b2 + nB) != -1)
							nB = {0, 1};
					}

					//due to off-by-one when multiplying we need to know which edge is at maxX and maxY and use next vertex in grid -1 pixel
					//we will use normals are directions for that
					int facA = facingOf(nA);
					int facB = facingOf(nB);
					ivec2 startA = a1 * size;
					if (dA.x == -1 || dA.y == -1) startA += dA;
					if (nA.x == 1 || nA.y == 1) startA -= nA;
					ivec2 startB = b1 * size;
					if (dB.x == -1 || dB.y == -1) startB += dB;
					if (nB.x == 1 || nB.y == 1) startB -= nB;

					//here we use ivec3 hash to encode {pos, facing}.
					//This is due to edge case that same corner can map to 2 warp destinations depending on facing
					for (int e = 0; e < size; e++) {
						ivec2 posA = startA + dA * e;
						ivec2 posB = startB + dB * e;
						ivec3 enterA = ivec3(posA.x, posA.y, facA);
						ivec3 enterB = ivec3(posB.x, posB.y, facB);
						ivec3 exitA = ivec3(posA.x, posA.y, (facA + 2) % 4 );
						ivec3 exitB = ivec3(posB.x, posB.y, (facB + 2) % 4 );
						auto [it, suc] = res.try_emplace(enterA, exitB);
						// if (!suc) std::cout << "duplicate warp at " << enterA.x << " " << enterA.y << " " << exitB.x << " " << exitB.y << std::endl;
						std::tie(it, suc) = res.try_emplace(enterB, exitA);
						// if (!suc) std::cout << "duplicate warp at " << enterB.x << " " << enterB.y << " " << exitA.x << " " << exitA.y << std::endl;
					}
					outEdges.erase(outEdges.begin() + i);
					outEdges.erase(outEdges.begin());
					break;
				}
			}
		}

		return res;
	}

	static void run() {


		std::ifstream inputStream("2022/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int mapY = 0;
		int inputStage = 0;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				inputStage++;
				continue;
			}
			if (inputStage == 0) {
				map.fillFromLine(mapY++, line, [](char ch) {
					switch (ch) {
						case '.': return 1;
						case '#': return 2;
						default: return 0;
					}
				});
			}
			else if (inputStage == 1) {
				pathStr = line;
			}
		}
		inputStream.close();\
		// map.print([](uint8_t val) {
		// 	if (val == 0) return ' ';
		// 	if (val == 2) return '#';
		// 	return '.';
		// });
		int instrPtr = 0;
		ivec2 pos = {0,0};
		for (int x=0; x<map.getWidth(); ++x) {
			if (map.get(x,0) == 1) {
				pos.x = x;
				break;
			}
		}
		int facing = 0;
		while (instrPtr < pathStr.size()) {
			int instr = captureInstruction(pathStr, instrPtr);
			if (instr == -1) facing = (facing + 1) % 4;
			else if (instr == -2) facing = (facing + 3) % 4;
			else {
				for (int i=0; i<instr; i++) {
					ivec2 d = facingVec[facing];
					ivec2 next = pos + d;
					if (!map.inRange(next.x, next.y) || map.get(next.x, next.y) == 0) {
						do {
							next -= d;
						}
						while (map.inRange(next.x, next.y) && map.get(next.x, next.y) != 0);
						next += d;
					}
					if (map.get(next.x, next.y) == 2) {
						break;
					}
					pos = next;
				}
			}
		}
		pos += {1,1};
		std::cout << 1000 * pos.y + pos.x * 4 + facing << std::endl;
	}
	static void runPart2() {
		const auto warps = warpMap(50);
		std::unordered_map<ivec2, int, ivec2hash> coordList;
		// for (auto key : std::views::keys(warps)) {
		// 	coordList[ivec2(key.x, key.y)] = key.z + 1;
		// }
		// DynamicGrid<int> warpKeys;
		// warpKeys.fillFromCoordList(coordList, [](ivec2 vec){return std::make_pair(vec.x, vec.y);});
		// warpKeys.print([](int val) {
		// 	if (val == 0)
		// 		return '.';
		// 	return char(val + '0');
		// });

		int instrPtr = 0;
		ivec2 pos = {0,0};
		for (int x=0; x<map.getWidth(); ++x) {
			if (map.get(x,0) == 1) {
				pos.x = x;
				break;
			}
		}
		int facing = 0;
		while (instrPtr < pathStr.size()) {
			int instr = captureInstruction(pathStr, instrPtr);
			if (instr == -1) facing = (facing + 1) % 4;
			else if (instr == -2) facing = (facing + 3) % 4;
			else {
				for (int i=0; i<instr; i++) {
					ivec2 d = facingVec[facing];
					ivec2 next = pos + d;
					int nextFacing = facing;
					if (!map.inRange(next.x, next.y) || map.get(next.x, next.y) == 0) {
						ivec3 key = ivec3{pos.x, pos.y, facing};
						ivec3 val = warps.at(key);
						nextFacing = val.z;
						next = ivec2{val.x, val.y};
					}
					if (map.get(next.x, next.y) == 2) {
						break;
					}
					pos = next;
					facing = nextFacing;
				}
			}
		}
		pos += {1,1};
		std::cout << 1000 * pos.y + pos.x * 4 + facing << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/