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
#include "utils.h"

//2 player fighting game

struct Task
{
	struct Item {
		std::string name;
		int cost;
		int damage;
		int armor;
	};
	inline static std::vector<Item> weapons {
	 {"Dagger",        8,     4,       0},
	 {"Shortsword",   10,     5,       0},
	 {"Warhammer",    25,     6,       0},
	 {"Longsword",    40,     7,       0},
	 {"Greataxe",     74,     8,       0}
	};
	inline static std::vector<Item> armor {
	 {"Leather",      13,     0,       1},
	 {"Chainmail",    31,     0,       2},
	 {"Splintmail",   53,     0,       3},
	 {"Bandedmail",   75,     0,       4},
	 {"Platemail",   102,     0,       5}
	};
	inline static std::vector<Item> rings {
		{"Damage +1",    25,     1,       0},
		{"Damage +2",    50,     2,       0},
		{"Damage +3",   100,     3,       0},
		{"Defense +1",   20,     0,       1},
		{"Defense +2",   40,     0,       2},
		{"Defense +3",   80,     0,       3}
	};
	static const int enemyHp = 100;
	static const int enemyDmg = 8;
	static const int enemyArm = 2;
	static const int playerHp = 100;

	template <typename Fn>
	requires std::invocable<Fn&, int, int, int>
	static void ForEachSet(Fn&& fn) {
		int totalCost = 0, totalDamage = 0, totalArmor = 0;
		std::vector<const Item*> inventory;
		auto pushItem = [&](const Item& item) {
			totalCost += item.cost;
			totalDamage += item.damage;
			totalArmor += item.armor;
			inventory.push_back(&item);
		};
		auto popItem = [&]() {
			totalCost -= inventory.back()->cost;
			totalDamage -= inventory.back()->damage;
			totalArmor -= inventory.back()->armor;
			inventory.pop_back();
		};
		for (const auto& weapon : weapons) {
			pushItem(weapon);
			for (int arm = 0; arm <= armor.size(); arm++) {
				if (arm!=armor.size()) pushItem(armor[arm]);
				for (int r1 = 0; r1 <= rings.size(); r1++) {
					if (r1!=rings.size()) pushItem(rings[r1]);
					for (int r2 = r1+1; r2 <= rings.size(); r2++) {
						if (r2<rings.size()) pushItem(rings[r2]);
						fn(totalCost, totalDamage, totalArmor);
						if (r2<rings.size()) popItem();
					}
					if (r1!=rings.size()) popItem();
				}
				if (arm!=armor.size()) popItem();
			}
			popItem();
		}
	}
	static void run() {

		int minCost = std::numeric_limits<int>::max();
		ForEachSet([&minCost](int cost, int damage, int armor) {
			int pHp = playerHp;
			int eHp = enemyHp;
			bool win = false;
			while (pHp > 0) {
				eHp -= std::max(damage - enemyArm, 1);
				if (eHp <= 0) {
					win = true;
					break;
				}
				pHp -= std::max(enemyDmg - armor, 1);
			}
			if (win && cost < minCost) {
				minCost = cost;
			}
		});
		std::cout << minCost << std::endl;
	}
	static void runPart2() {
		int maxCost = 0;
		ForEachSet([&maxCost](int cost, int damage, int armor) {
			int pHp = playerHp;
			int eHp = enemyHp;
			bool win = false;
			while (pHp > 0) {
				eHp -= std::max(damage - enemyArm, 1);
				if (eHp <= 0) {
					win = true;
					break;
				}
				pHp -= std::max(enemyDmg - armor, 1);
			}
			if (!win && cost > maxCost) {
				maxCost = cost;
			}
		});
		std::cout << maxCost << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/