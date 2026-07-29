#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"

//colliding carts and rails

struct CellInfo {
	uint8_t data = 0;
	bool hasCart() const {return data & 0b1;}
	bool canMove(int dir) const {return (data >> (1 + dir)) & 1;}
	bool isCrossroad() const {return (data >> 1) == 0b1111;}
	uint8_t moveset() const {return (data >> 1) & 0b1111; }

	void setCart(bool val){ data = (data & ~0b1) | val;}
	void setMove(int dir, bool val) {data = (data & ~(0b10 << dir)) | (val << (1 + dir));}
};
struct Cart {
	int nextTurn = 1;
	int x;
	int y;
	int dir = 0;
	int getNextDir(const CellInfo& info) {
		if (info.isCrossroad()) {
			int newDir = Utils::FloorMod(dir + nextTurn, 4);
			nextTurn--;
			if (nextTurn == -2) nextTurn = 1;
			return newDir;
		}
		uint8_t moveset = info.moveset();
		moveset &= ~ (1 << ((dir + 2) & 0b11));
		int code = 1;
		for (int i=0; i<4; i++) {
			if (code == moveset)
				return i;
			code <<= 1;
		}
		return -1;
	}
};
struct Task
{
	static inline std::vector<Cart> carts;
	static inline DynamicGrid<CellInfo> map;
	static inline std::vector<Cart> backup;

	static bool advance(int& crashX, int& crashY) {
		std::sort(carts.begin(), carts.end(), [](const Cart& c1, const Cart& c2) {
			if (c1.y != c2.y) return c1.y < c2.y;
			return c1.x < c2.x;
		});
		for (int i = 0; i<carts.size(); i++) {
			auto& c = carts[i];
			int oldX = c.x;
			int oldY = c.y;
			c.dir = c.getNextDir(map.get(c.x, c.y));
			map.moveInDir(c.x, c.y, c.dir);
			if (map.get(c.x, c.y).hasCart()) {
				crashX = c.x;
				crashY = c.y;
				return false;
			}
			map.getRef(oldX, oldY).setCart(false);
			map.getRef(c.x, c.y).setCart(true);
		}

		// map.print([](const CellInfo& ci) {
		// 	if (ci.hasCart()) return '#';
		// 	switch (ci.moveset()) {
		// 		case 0b0101: return '-';
		// 		case 0b1010: return '|';
		// 		case 0b1111: return '+';
		// 		case 0b1100:
		// 		case 0b0011: return '\\';
		// 		case 0b1001:
		// 		case 0b0110: return '/';
		// 		default: return '.';
		// 	}
		// });
		// std::cout << "---------------" << std::endl;
		return true;
	}

	static bool advanceDestructive() {
		std::sort(carts.begin(), carts.end(), [](const Cart& c1, const Cart& c2) {
			if (c1.y != c2.y) return c1.y < c2.y;
			return c1.x < c2.x;
		});
		std::set<int, std::greater<int>> toRemove;
		for (int i = 0; i<carts.size(); i++) {
			auto& c = carts[i];
			int oldX = c.x;
			int oldY = c.y;
			c.dir = c.getNextDir(map.get(c.x, c.y));
			map.moveInDir(c.x, c.y, c.dir);
			if (map.get(c.x, c.y).hasCart()) {
				for (int j=0; j<carts.size(); j++) {
					if (i == j) continue;
					if (carts[j].x == c.x && carts[j].y == c.y) {
						toRemove.emplace(j);
						toRemove.emplace(i);
					}
				}
			}
			map.getRef(oldX, oldY).setCart(false);
			map.getRef(c.x, c.y).setCart(true);
		}
		if (toRemove.empty()) return true;
		// std::sort(toRemove.begin(), toRemove.end(), std::less<int>());
		for (auto& i : toRemove) {
			carts.erase(carts.begin()+i);
		}
		if (carts.size() == 1) return false;

		// map.print([](const CellInfo& ci) {
		// 	if (ci.hasCart()) return '#';
		// 	switch (ci.moveset()) {
		// 		case 0b0101: return '-';
		// 		case 0b1010: return '|';
		// 		case 0b1111: return '+';
		// 		case 0b1100:
		// 		case 0b0011: return '\\';
		// 		case 0b1001:
		// 		case 0b0110: return '/';
		// 		default: return '.';
		// 	}
		// });
		// std::cout << "---------------" << std::endl;
		return true;
	}

	static void run() {

		std::ifstream inputStream("2018/13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;

		int y = 0;
		bool connectPrevious = false;
		while (std::getline(inputStream, line))
		{
			connectPrevious = false;
			for (int x=0; x<line.length(); ++x) {
				CellInfo ci;
				if (line[x] == ' ') {
					continue;
				}
				else if (line[x] == '-') {
					ci.setMove(0, true);
					ci.setMove(2, true);
					connectPrevious = true;
				}
				else if (line[x] == '|') {
					ci.setMove(1, true);
					ci.setMove(3, true);
					connectPrevious = false;
				}
				else if (line[x] == '+') {
					ci.setMove(0, true);
					ci.setMove(1, true);
					ci.setMove(2, true);
					ci.setMove(3, true);
					connectPrevious = true;
				}
				else if (line[x] == '/') {
					if (connectPrevious) {
						ci.setMove(1, true);
						ci.setMove(2, true);
					}
					else {
						ci.setMove(0, true);
						ci.setMove(3, true);
					}
					connectPrevious = !connectPrevious;
				}
				else if (line[x] == '\\') {
					if (connectPrevious) {
						ci.setMove(3, true);
						ci.setMove(2, true);
					}
					else {
						ci.setMove(0, true);
						ci.setMove(1, true);
					}
					connectPrevious = !connectPrevious;
				}
				else {
					Cart cart;
					cart.x = x;
					cart.y = y;
					if (line[x] == '>') {
						ci.setCart(true);
						cart.dir = 0;
						ci.setMove(0, true);
						ci.setMove(2, true);
						connectPrevious = true;
					}
					else if (line[x] == '<') {
						ci.setCart(true);
						cart.dir = 2;
						ci.setMove(0, true);
						ci.setMove(2, true);
						connectPrevious = true;
					}
					else if (line[x] == '^') {
						ci.setCart(true);
						cart.dir = 1;
						ci.setMove(1, true);
						ci.setMove(3, true);
						connectPrevious = false;
					}
					else if (line[x] == 'v') {
						ci.setCart(true);
						cart.dir = 3;
						ci.setMove(1, true);
						ci.setMove(3, true);
						connectPrevious = false;
					}
					carts.push_back(cart);
				}
				map.add(x,y,ci);
			}
			y++;
		}
		inputStream.close();
		backup = carts;
		int crashX, crashY;
		while (advance(crashX, crashY));
		for (auto& cart : carts) {
			map.getRef(cart.x, cart.y).setCart(false);
		}
		std::cout << crashX << ',' << crashY << std::endl;
	}
	static void runPart2() {
		carts = backup;
		for (auto& cart : carts) {
			map.getRef(cart.x, cart.y).setCart(true);
		}
		while (advanceDestructive());
		std::cout << carts[0].x << ',' << carts[0].y << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/