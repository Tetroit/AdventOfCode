#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <map>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"
#include "vec.h"

//oh no.. it is pathfinding with keys and dors

struct Lock {
	ivec2 key;
	ivec2 door;
	char type;
	std::unordered_set<char> req;
};
struct Task {
	static inline std::unordered_map<char, Lock> locks;
	static inline std::unordered_map<char,std::unordered_map<char, int>> graph;
	static inline std::unordered_map<std::string, std::unordered_map<char,int>> cache;
	static inline std::unordered_map<std::string, std::unordered_map<std::string,int>> cache2;
	static inline std::array<std::unordered_set<char>, 4> sectors;
	static inline std::unordered_map<char, int> sectorOf;
	static inline DynamicGrid<char> grid;

	static void registerRes(std::vector<char> acquiredKeys, int res) {
		char last = acquiredKeys.back();
		std::sort(acquiredKeys.begin(), acquiredKeys.end());
		std::string key(acquiredKeys.begin(), acquiredKeys.end());
		cache[key][last] = res;
	}
	static void registerRes2(std::vector<char> acquiredKeys, std::array<char, 4> pos, int res) {
		std::sort(acquiredKeys.begin(), acquiredKeys.end());
		std::string key(acquiredKeys.begin(), acquiredKeys.end());
		std::string posKey(pos.begin(), pos.end());
		cache2[key][posKey] = res;
	}
	static std::optional<int> getCache(std::vector<char> acquiredKeys) {
		char last = acquiredKeys.back();
		std::sort(acquiredKeys.begin(), acquiredKeys.end());
		std::string key(acquiredKeys.begin(), acquiredKeys.end());
		if (!cache.contains(key)) return std::nullopt;
		auto& storage = cache.at(key);
		if (!storage.contains(last)) return std::nullopt;
		return storage.at(last);
	}
	static std::optional<int> getCache2(std::vector<char> acquiredKeys, std::array<char, 4> pos) {
		std::sort(acquiredKeys.begin(), acquiredKeys.end());
		std::string key(acquiredKeys.begin(), acquiredKeys.end());
		std::string posKey(pos.begin(), pos.end());
		if (!cache2.contains(key)) return std::nullopt;
		auto& storage = cache2.at(key);
		if (!storage.contains(posKey)) return std::nullopt;
		return storage.at(posKey);
	}

	static void buildGraph(ivec2 start) {
		for (auto& [type, lock] : locks) {
			auto& dsts = graph[type];
			dsts.clear();
			auto keyPos = locks[type].key;
			auto weights = grid.dijkstra(keyPos.x, keyPos.y,
		[](char ch){return 1;},
		[&](char ch) { return ch == '#';});
			for (auto& [otherT, other] : locks) {
				if (otherT == type) continue;
				auto otherKey = locks[otherT].key;
				dsts[otherT] = weights.get(otherKey.x, otherKey.y);
			}
		}
	}
	static void findLocks(ivec2 start) {
		for (auto& [type, lock] : locks) {
			auto weights = grid.dijkstra(start.x, start.y,
				[](char ch){return 1;},
		[&](char ch) {
					return ch == '#' || ch == toupper(type);
				});
			for (auto& [otherT, other] : locks) {
				if (otherT == type) continue;
				if (weights.get(other.key.x, other.key.y) == INT_MAX) {
					other.req.emplace(type);
				}
			}
		}
	}
	static void buildGraph2(std::array<ivec2, 4> starts) {

		sectorOf.clear();
		for (auto& arr : sectors) {
			arr.clear();
		}
		for (auto& [type, lock] : locks) {
			auto& dsts = graph[type];
			dsts.clear();
			auto keyPos = locks[type].key;
			auto weights = grid.dijkstra(keyPos.x, keyPos.y,
		[](char ch){return 1;},
		[&](char ch) { return ch == '#';});
			for (auto& [otherT, other] : locks) {
				if (otherT == type) continue;
				auto otherKey = locks[otherT].key;
				int path = weights.get(otherKey.x, otherKey.y);
				if (path != INT_MAX) {
					dsts[otherT] = path;
				}
			}

			for (int i=0; i < 4; i++) {
				ivec2& istart = starts[i];
				int path = weights.get(istart.x, istart.y);
				if (path != INT_MAX) {
					sectors[i].emplace(type);
					sectorOf[type] = i;
					graph['0'+i][type] = path;
				}
			}
		}
	}
	static int keySequenceCached(std::vector<char>& acquiredKeys, ivec2 start) {
		std::unordered_map<char, Lock> currentState = locks;
		std::vector<char> priority;
		auto weights = grid.dijkstra(start.x, start.y,
			[](char ch){return 1;},
			[&](char ch) {
				if (std::isupper(ch)) {return false;}
				return ch == '#';
			});
		int minPath = INT_MAX;
		for (auto& [type, lock] : currentState) {
			if (!lock.req.empty()) continue;
			int path = weights.get(lock.key.x, lock.key.y);
			if (path == INT_MAX)
				continue;
			acquiredKeys.push_back(type);
			auto newState = currentState;
			newState.erase(type);
			for (auto& [_, newLock] : newState) {
				newLock.req.erase(type);
			}
			int candidate = keySequenceCached(acquiredKeys, type, newState) + path;
			if (candidate < minPath) minPath = candidate;

			acquiredKeys.pop_back();
		}
		return minPath;

	}
	static int keySequenceCached(std::vector<char>& acquiredKeys, char current, std::unordered_map<char, Lock>& currentState) {

		std::vector<char> priority;
		if (currentState.size() == 1) {
			auto it = currentState.begin();
			// for (auto& key : acquiredKeys) {
			// 	std::cout << key;
			// }
			// std::cout << it->first << std::endl;
			return graph[current][it->first];
		}
		int minPath = INT_MAX;
		for (auto& [type, lock] : currentState) {
			if (!lock.req.empty()) continue;
			int path = graph[current][type];
			if (path == INT_MAX)
				continue;
			acquiredKeys.push_back(type);

			int candidate;
			if (auto val = getCache(acquiredKeys); val.has_value())
				candidate = val.value() + path;
			else {
				auto newState = currentState;
				newState.erase(type);
				for (auto& [_, newLock] : newState) {
					newLock.req.erase(type);
				}
				candidate = keySequenceCached(acquiredKeys, type, newState) + path;
			}

			if (candidate < minPath) minPath = candidate;

			acquiredKeys.pop_back();
		}
		registerRes(acquiredKeys, minPath);
		return minPath;

	}

	static int keySequenceCached2(std::vector<char>& acquiredKeys, std::array<char, 4> currentPos, std::unordered_map<char, Lock>& currentLocks) {

		std::vector<char> priority;
		if (currentLocks.size() == 1) {
			auto it = currentLocks.begin();
			// for (auto& key : acquiredKeys) {
			// 	std::cout << key;
			// }
			// std::cout << it->first << std::endl;
			return graph[currentPos[sectorOf[it->first]]][it->first];
		}
		int minPath = INT_MAX;
		for (auto& [type, lock] : currentLocks) {
			if (!lock.req.empty()) continue;
			int sector = sectorOf[type];
			char currentNode = currentPos[sector];
			int path = graph[currentNode][type];
			if (path == INT_MAX)
				continue;

			acquiredKeys.push_back(type);
			auto newPos = currentPos;
			newPos[sector] = type;
			int candidate;
			if (auto val = getCache2(acquiredKeys, newPos); val.has_value())
				candidate = val.value() + path;
			else {
				auto newState = currentLocks;
				newState.erase(type);
				for (auto& [_, newLock] : newState) {
					newLock.req.erase(type);
				}
				candidate = keySequenceCached2(acquiredKeys, newPos, newState) + path;
			}

			if (candidate < minPath) minPath = candidate;

			acquiredKeys.pop_back();
		}
		registerRes2(acquiredKeys, currentPos, minPath);
		return minPath;

	}
	static inline ivec2 startPos;
	static void run() {

		std::ifstream inputStream("2019/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int readY = 0;
		while (std::getline(inputStream, line))
		{
			grid.fillFromLine(readY, line, DefaultConvert<char>());
			readY++;

		}
		inputStream.close();
		;
		grid.foreachCoord([&](int x, int y) {
			auto val = grid.get(x, y);
			if (val == '@') {
				startPos = {x,y};
			}
			if (val >= 'a' && val <= 'z') {
				locks[val].key = ivec2(x, y);
				locks[val].type = val;
			}
			else if (val >= 'A' && val <= 'Z') {
				char lock = std::tolower(val);
				locks[lock].door = ivec2(x, y);
			}
		});
		std::vector<char> acquiredKeys;
		findLocks(startPos);
		buildGraph(startPos);
		std::cout << keySequenceCached(acquiredKeys, startPos) << std::endl;
	}
	static void runPart2() {
		grid.set(startPos.x, startPos.y, '#');
		grid.set(startPos.x-1, startPos.y, '#');
		grid.set(startPos.x+1, startPos.y, '#');
		grid.set(startPos.x, startPos.y-1, '#');
		grid.set(startPos.x, startPos.y+1, '#');
		grid.set(startPos.x-1, startPos.y-1, '0');
		grid.set(startPos.x+1, startPos.y-1, '1');
		grid.set(startPos.x-1, startPos.y+1, '2');
		grid.set(startPos.x+1, startPos.y+1, '3');

		graph.clear();
		std::array<ivec2, 4> starts = {
			startPos + ivec2{-1,-1},
			startPos + ivec2{1,-1},
			startPos + ivec2{-1,1},
			startPos + ivec2{1,1}
		};
		buildGraph2(starts);
		std::array<char, 4> nodes = {'0','1','2','3'};

		std::vector<char> acquiredKeys;
		auto startLocks = locks;
		std::cout << keySequenceCached2(acquiredKeys, nodes, startLocks) << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

thanks god maze has no loops
CACHE EVERYTHING!

*/