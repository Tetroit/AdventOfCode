#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <stack>
#include <stack>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "Intcode.h"
#include "vec.h"

//droid walking in labyrinth

struct Task
{
	static inline Intcode<int> program;
	static inline std::unordered_map<ivec2, int, ivec2hash> map;
	static inline std::unordered_map<ivec2, int, ivec2hash> ruler;
	static inline DynamicGrid<int> display;

	static inline const std::vector<int> oppositeDir{0, 2, 1, 4, 3};
	static inline ivec2 start;
	static inline ivec2 end;
	static void run() {

		static constexpr bool MANUAL_MODE = false;
		std::ifstream inputStream("2019/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		program = Intcode<int>(inputStream);

		inputStream.close();

		std::deque<int> input;
		std::deque<int> output;
		HaltCode code;

		program.start();
		int dir;
		ivec2 pos = {0,0};
		map.emplace(ivec2{0,0}, 4);
		int repeats = 1;
		bool explore = false;
		bool smartMove = false;
		int steps = 0;

		ruler[{0,0}]=0;
		const auto execute = [&](int aDir) {

			input.push_back(aDir);
			while (program.executeUntilInput(input, output, code));

			int res = output.front();
			output.pop_front();
			ivec2 newPos = pos;
			switch (aDir) {
				case 1: newPos.y--; break;
				case 2: newPos.y++; break;
				case 3:	newPos.x--; break;
				case 4:	newPos.x++; break;
				default: break;
			}
			map[newPos] = res+1;
			if (res == 1 || res == 2) {
				map[pos] = map[pos] == 3 ? 3 : 2;
				pos = newPos;
				map[pos] = res == 2 ? 3 : 4;
				steps++;
				if (!ruler.contains(pos) || ruler[pos] > steps) ruler[pos] = steps;
				else if (ruler[pos] < steps) steps = ruler[pos];
			}
			return res;
		};
		if (MANUAL_MODE) {
			while (true) {
				std::cin >> dir;
				if (dir == -1) break;
				if (dir == 5) {
					std::cin >> repeats;
					std::cout << "Set repeats to " << repeats << "\n";
					continue;
				}
				if (dir == 6) {
					explore = !explore;
					std::cout << (explore ? "Exploration enabled" : "Exploration disabled") << std::endl;
					continue;
				}
				if (dir == 7) {
					smartMove = !smartMove;
					std::cout << (smartMove ? "Smart movement enabled" : "Smart movement disabled") << std::endl;
					continue;
				}
				if (dir == 8) {
					std::cout << "Steps from start: " << steps << std::endl;
					continue;
				}
				if (dir == 9) {
					std::vector<ivec2> path{{0,0}};
					const std::vector<ivec2> dirToDelta{{0,0}, {0,-1}, {0, 1}, {-1, 0}, {1,0}};;
					const std::unordered_map<ivec2, int, ivec2hash> deltaToDir{
							{{0,-1}, 1},
							{{0, 1}, 2},
							{{-1, 0}, 3},
							{{1,0}, 4}
					};
					while (true) {
						auto node = path.back();
						bool explored = true;
						for (int i=1; i<=4; i++) {
							auto candidate = node + dirToDelta.at(i);
							if (path.size() > 1 && path[path.size() - 2] == candidate) continue;
							if (!map.contains(candidate)) {
								int res = execute(i);
								if (res != 0) {
									path.push_back(candidate);
									explored = false;
									break;
								}
							}
						}
						if (explored) {
							path.pop_back();
							if (path.empty())
								break;
							execute(deltaToDir.at( path.back() - node));
						}
					}
				}

				else if (smartMove) {
					int currentDir = dir;
					int backDir = oppositeDir[currentDir];
					int possibleMoves = 0;
					while (possibleMoves < 2) {
						backDir = oppositeDir[currentDir];
						execute(currentDir);

						possibleMoves = 0;
						for (int d=1; d<=4; d++) {
							if (d == backDir) continue;
							const int res = execute(d);
							if (res != 0) {
								execute (oppositeDir[d]);
								possibleMoves++;
								currentDir = d;
							}
						}
						if (possibleMoves == 0) {
							currentDir = backDir;
						}
					}
				}
				else {
					for (int i = 0; i < repeats; i++) {
						execute(dir);
						if (explore) {
							for (int d=1; d<=4; d++) {
								if (const int res = execute(d); res != 0) execute (oppositeDir[d]);
							}
						}
					}
				}
				display.fillFromCoordList(map, [](ivec2 vec){return std::make_pair(vec.x,vec.y);});
				display.print([](int val) {
					switch (val) {
						case 0: return ' ';
						case 1: return '#';
						case 2: return '.';
						case 3: return '0';
						case 4: return 'v';
						default: return '_';
					}
				});
			}
		}
		else {
			std::vector<ivec2> path{{0,0}};
			const std::vector<ivec2> dirToDelta{{0,0}, {0,-1}, {0, 1}, {-1, 0}, {1,0}};;
			const std::unordered_map<ivec2, int, ivec2hash> deltaToDir{
								{{0,-1}, 1},
								{{0, 1}, 2},
								{{-1, 0}, 3},
								{{1,0}, 4}
			};
			while (true) {
				auto node = path.back();
				bool explored = true;
				for (int i=1; i<=4; i++) {
					auto candidate = node + dirToDelta.at(i);
					if (path.size() > 1 && path[path.size() - 2] == candidate) continue;
					if (!map.contains(candidate)) {
						int res = execute(i);
						if (res != 0) {
							path.push_back(candidate);
							explored = false;
							break;
						}
					}
				}
				if (explored) {
					path.pop_back();
					if (path.empty())
						break;
					execute(deltaToDir.at( path.back() - node));
				}
			}
		}
		program.end();

		display.fillFromCoordList(map, [](ivec2 vec){return std::make_pair(vec.x,vec.y);});
		// display.print([](int val) {
		// 	switch (val) {
		// 		case 0: return ' ';
		// 		case 1: return '#';
		// 		case 2: return '.';
		// 		case 3: return '0';
		// 		case 4: return 'v';
		// 		default: return '_';
		// 	}
		// });

		display.foreachCoord([](int x, int y) {
			if (display.get(x,y) == 4) start = {x,y};
			if (display.get(x,y) == 3) end = {x,y};
		});
		int shortestPath = display.dijkstra(start.x, start.y, end.x, end.y,
			[](int i){return 1;},
			[](int i){return i==1;});

		std::cout << shortestPath << std::endl;
	}
	static void runPart2() {
		auto weights = display.dijkstra(end.x, end.y,
			[](int i){return 1;},
			[](int i){return i==1;});
		int maxWeight = 0;
		weights.foreachCoord([&](int x, int y) {
			if (weights.get(x,y) == INT_MAX) return;
			if (maxWeight < weights.get(x,y)) maxWeight = weights.get(x,y);
		});
		std::cout <<  maxWeight << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/