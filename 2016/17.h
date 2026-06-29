#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "quickdigest5.hpp"
#include "utils.h"

//walking through rooms via md5
struct Task
{
	static inline const std::string pass = "yjjvjgan";
	static inline const std::string dirStr = "UDLR";
	enum class Dir {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
	};
	struct Node {
		std::string path;
		int x, y;

	};
	static bool CanMove(Dir dir, const int& x, const int& y) {
		if (dir == Dir::UP && y>0) {
			return true;
		}
		if (dir == Dir::DOWN && y<3) {
			return true;
		}
		if (dir == Dir::RIGHT && x<3) {
			return true;
		}
		if (dir == Dir::LEFT && x>0) {
			return true;
		}
		return false;
	}
	static void Move(Dir dir, int& x, int& y) {
		switch (dir) {
			case Dir::UP: {
				y--;
				break;
			}
			case Dir::DOWN: {
				y++;
				break;
			}
			case Dir::LEFT: {
				x--;
				break;
			}
			case Dir::RIGHT: {
				x++;
				break;
			}
		}
	}
	static std::array<bool, 4> readDirections(const std::vector<uint8_t>& data) {
		std::array<bool, 4> directions;
		for (int i=0; i<4; i++) {
			int cell = data[i/2];
			uint8_t code = i&1 ? cell & 15 : cell >> 4;
			directions[i] = (code >= 0xb);
		}
		return directions;
	}
	static void run() {
		std::deque<Node> paths{Node("", 0, 0)};
		while (true) {
			if (paths.empty()) {
				break;
			}
			Node current = paths.front();
			paths.pop_front();
			auto md5 = QuickDigest5::digestString(pass + current.path);
			auto opens = readDirections(md5);
			for (int i=0; i<4; i++) {
				if (opens[i] && CanMove((Dir)i, current.x, current.y)) {
					Node newNode = current;
					Move((Dir)i, newNode.x, newNode.y);
					newNode.path += dirStr[i];
					if (newNode.x == 3 && newNode.y == 3) {
						std::cout << newNode.path << std::endl;
						return;
					}
					paths.push_back(newNode);
				}
			}
		}
	}
	static void runPart2() {
		int longest = 0;
		std::deque<Node> paths{Node("", 0, 0)};
		while (true) {
			if (paths.empty()) {
				break;
			}
			Node current = paths.front();
			paths.pop_front();
			auto md5 = QuickDigest5::digestString(pass + current.path);
			auto opens = readDirections(md5);
			for (int i=0; i<4; i++) {
				if (opens[i] && CanMove((Dir)i, current.x, current.y)) {
					Node newNode = current;
					Move((Dir)i, newNode.x, newNode.y);
					newNode.path += dirStr[i];
					if (newNode.x == 3 && newNode.y == 3) {
						// std::cout << newNode.path << std::endl;
						longest = std::max(longest, (int)newNode.path.length());
						continue;
					}
					paths.push_back(newNode);
				}
			}
		}
		std::cout << longest << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/