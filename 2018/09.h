#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <list>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//placing marbles in a circle, removing 23th marble
struct Task
{
	static constexpr int players = 465;
	static constexpr int last = 71940;

	static void loopPrev(const std::list<int>& list, std::list<int>::const_iterator& it, int n) {
		for (int i = 0; i < n; ++i) {
			if (it == list.begin()) it = list.end();
			--it;
		}
	}
	static void loopNext(const std::list<int>& list, std::list<int>::const_iterator& it, int n) {
		for (int i = 0; i < n; ++i) {
			if (it == list.end()) it = list.begin();
			++it;
		}
	}
	static void run() {
		std::list<int> marbles{0};
		std::vector<long long> scores(players, 0);
		int player = 1;
		auto current = marbles.cbegin();
		for (int i = 1; i <= last; ++i) {
			if (i%23 == 0) {
				loopPrev(marbles, current, 7);
				long long score = *current + i;
				if (current == marbles.cend()) current = marbles.cbegin();
				current = marbles.erase(current);
				scores[player-1] += score;
			}
			else {
				loopNext(marbles, current, 2);
				current = marbles.insert(current, i);
			}

			player++;
			if (player>players) player -= players;
		}
		long long maxScore = 0;
		for (auto score : scores) {
			if (score > maxScore) maxScore = score;
		}
		std::cout << maxScore << std::endl;
	}
	static void runPart2() {
		std::list<int> marbles{0};
		std::vector<long long> scores(players, 0);
		int player = 1;
		auto current = marbles.cbegin();
		for (int i = 1; i <= last * 100; ++i) {
			if (i%23 == 0) {
				loopPrev(marbles, current, 7);
				long long score = *current + i;
				if (current == marbles.cend()) current = marbles.cbegin();
				current = marbles.erase(current);
				scores[player-1] += score;
			}
			else {
				loopNext(marbles, current, 2);
				current = marbles.insert(current, i);
			}

			player++;
			if (player>players) player -= players;
		}
		long long maxScore = 0;
		for (auto score : scores) {
			if (score > maxScore) maxScore = score;
		}
		std::cout << maxScore << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

std::vector has O(1) random access but O(n) insert/erase

std::list has O(n) random access but O(1) insert/erase

here we dont need random access
we track position with iterator going back and forth and can retrieve its value any time
however we do 1 insert/erase per iteration
so use std::list

*/