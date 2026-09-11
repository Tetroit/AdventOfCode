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

//playing durak but keeping cards

struct Task
{
	static inline std::deque<int> deck1;
	static inline std::deque<int> deck2;

	struct State {
		std::deque<int> d1;
		std::deque<int> d2;

		bool operator==(const State& other) const {
			return d1 == other.d1 && d2 == other.d2;
		}
	};
	struct StateHash {
		size_t operator()(const State &s) const {
			std::hash<int> hasher;
			size_t result = 0;
			for (int i : s.d1) {
				result <<=1;
				result ^= hasher(i);
			}
			for (int i : s.d2) {
				result <<=1;
				result ^= hasher(i);
			}
			return result;
		}
	};

	static int recurseGame(std::deque<int>& d1, std::deque<int>& d2) {
		std::unordered_set<State, StateHash> records;
		while (!d1.empty() && !d2.empty()) {

			if (records.contains({d1, d2})) return 1;
			records.emplace(d1, d2);
			int p1 = d1.front();
			int p2 = d2.front();
			d1.pop_front();
			d2.pop_front();
			int roundWinner = 0;
			if (d1.size() >= p1 && d2.size() >= p2) {
				std::deque<int> new1;
				std::deque<int> new2;
				auto it1 = d1.begin();
				for (int i = 0; i<p1; i++) {
					new1.push_back(*it1);
					it1++;
				}
				auto it2 = d2.begin();
				for (int i = 0; i<p2; i++) {
					new2.push_back(*it2);
					it2++;
				}
				roundWinner = recurseGame(new1, new2);
			}
			else
				roundWinner = p1 > p2 ? 1 : 2;

			if (roundWinner == 1) {
				d1.push_back(p1);
				d1.push_back(p2);
			}
			else if (roundWinner == 2) {
				d2.push_back(p2);
				d2.push_back(p1);
			}

		}
		return d2.empty() ? 1 : 2;
	}
	static void run() {

		std::ifstream inputStream("2020/22.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		bool secondDeck = false;
		std::getline(inputStream, line);
		while (std::getline(inputStream, line))
		{
			if (line == "") continue;
			if (line == "Player 2:") {
				secondDeck = true;
				continue;
			}
			if (!secondDeck) {
				deck1.push_back(std::stoi(line));
			}
			else {
				deck2.push_back(std::stoi(line));
			}
		}
		inputStream.close();
		auto playingDeck1 = deck1;
		auto playingDeck2 = deck2;
		while (playingDeck1.size() > 0 && playingDeck2.size() > 0) {
			int p1 = playingDeck1.front();
			int p2 = playingDeck2.front();
			playingDeck1.pop_front();
			playingDeck2.pop_front();
			if (p1 > p2) {
				playingDeck1.push_back(p1);
				playingDeck1.push_back(p2);
			}
			else {
				playingDeck2.push_back(p2);
				playingDeck2.push_back(p1);
			}
		}
		auto* winner = !playingDeck1.empty() ? &playingDeck1 : &playingDeck2;
		int i=winner->size();
		int score = 0;
		for (auto it = winner->begin(); it != winner->end(); it++) {
			score += *it * i;
			i--;
		}
		std::cout << score << std::endl;
	}
	static void runPart2() {

		auto d1 = deck1;
		auto d2 = deck2;
		std::unordered_set<State, StateHash> records;
		bool repeat = false;
		while (!d1.empty() && !d2.empty()) {

			if (records.contains({d1, d2})){
				repeat = true;
				break;
			}
			records.emplace(d1, d2);

			int p1 = d1.front();
			int p2 = d2.front();
			d1.pop_front();
			d2.pop_front();
			int roundWinner = 0;
			if (d1.size() >= p1 && d2.size() >= p2) {
				std::deque<int> new1;
				std::deque<int> new2;
				auto it1 = d1.begin();
				for (int i = 0; i<p1; i++) {
					new1.push_back(*it1);
					it1++;
				}
				auto it2 = d2.begin();
				for (int i = 0; i<p2; i++) {
					new2.push_back(*it2);
					it2++;
				}
				roundWinner = recurseGame(new1, new2);
			}
			else
				roundWinner = p1 > p2 ? 1 : 2;

			if (roundWinner == 1) {
				d1.push_back(p1);
				d1.push_back(p2);
			}
			else if (roundWinner == 2) {
				d2.push_back(p2);
				d2.push_back(p1);
			}
		}
		std::deque<int>* winner;
		if (repeat) winner = &d1;
		else winner = !d1.empty() ? &d1 : &d2;

		int i=winner->size();
		int score = 0;
		for (auto it = winner->begin(); it != winner->end(); it++) {
			score += *it * i;
			i--;
		}
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/