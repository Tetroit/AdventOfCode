#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <ranges>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//playing poker

struct Task
{
	enum struct HandType {
		High = 0,
		Pair = 1,
		TwoPair = 2,
		Three = 3,
		FullHouse = 4,
		Four = 5,
		Five = 6
	};
	struct CardCompare {
		static int getScore(char ch) {
			if (ch == 'T') return 10;
			if (ch == 'J') return 11;
			if (ch == 'Q') return 12;
			if (ch == 'K') return 13;
			if (ch == 'A') return 14;
			return ch - '0';
		}

		bool operator()(const char a, const char b) const {
			return getScore(a) > getScore(b);
		}
	};
	struct CardCompare2 {
		static int getScore(char ch) {
			if (ch == 'T') return 10;
			if (ch == 'J') return 1;
			if (ch == 'Q') return 12;
			if (ch == 'K') return 13;
			if (ch == 'A') return 14;
			return ch - '0';
		}

		bool operator()(const char a, const char b) const {
			return getScore(a) > getScore(b);
		}
	};

	struct HandIdentifier {
		HandType operator()(const std::string& hand) const {
			std::unordered_map<char, int> freq;
			std::vector<int> groups(6,0);
			for (auto& ch : hand) {
				freq[ch]++;
			}
			for (const auto& cnt : freq | std::views::values) {
				groups[cnt]++;
			}
			if (groups[5] == 1) return HandType::Five;
			if (groups[4] == 1) return HandType::Four;
			if (groups[3] == 1 && groups[2] == 1) return HandType::FullHouse;
			if (groups[3] == 1) return HandType::Three;
			if (groups[2] == 2) return HandType::TwoPair;
			if (groups[2] == 1) return HandType::Pair;
			return HandType::High;
		}
	};
	struct HandIdentifier2 {
		HandType operator()(const std::string& hand) const {
			std::unordered_map<char, int> freq;
			std::vector<int> groups(6,0);
			int jokers = 0;
			for (auto& ch : hand) {
				if (ch == 'J') {
					jokers++;
					continue;
				}
				freq[ch]++;
			}
			int max = 0;
			for (const auto& cnt : freq | std::views::values) {
				if (cnt > max) max = cnt;
				groups[cnt]++;
			}
			groups[max]--;
			groups[max + jokers]++;
			if (groups[5] == 1) return HandType::Five;
			if (groups[4] == 1) return HandType::Four;
			if (groups[3] == 1 && groups[2] == 1) return HandType::FullHouse;
			if (groups[3] == 1) return HandType::Three;
			if (groups[2] == 2) return HandType::TwoPair;
			if (groups[2] == 1) return HandType::Pair;
			return HandType::High;
		}
	};

	template <typename CardComp, typename HandIdent>
	struct HandCompare {
		bool operator()(const std::string& a, const std::string& b) const {
			int comboA = (int)HandIdent()(a);
			int comboB = (int)HandIdent()(b);
			if (comboA != comboB) return comboA > comboB;
			for (int i=0; i<5; i++) {
				int cardA = CardComp::getScore(a[i]);
				int cardB = CardComp::getScore(b[i]);
				if (cardA != cardB) return cardA > cardB;
			}
			return false;
		}
	};
	static inline std::vector<std::pair<std::string, int>> hands;
	static void run() {

		std::ifstream inputStream("2023/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string hand;
		int weight = 0;
		while (inputStream >> hand >> weight)
		{
			hands.emplace_back(hand, weight);
		}
		inputStream.close();


		std::sort(hands.begin(), hands.end(), [](std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
			return !HandCompare<CardCompare, HandIdentifier>()(a.first, b.first);
		});
		int total = 0;
		for (int i=0; i<hands.size(); i++) {
			total += hands[i].second * (i + 1);
		}
		std::cout << total << std::endl;
	}
	static void runPart2() {
		std::sort(hands.begin(), hands.end(), [](std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
			return !HandCompare<CardCompare2, HandIdentifier2>()(a.first, b.first);
		});
		int total = 0;
		for (int i=0; i<hands.size(); i++) {
			total += hands[i].second * (i + 1);
		}
		std::cout << total << std::endl;
		//252041216 high
	}
};

//-------------- NOTES AREA ----------------
/*

*/