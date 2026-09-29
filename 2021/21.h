#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//game with rolling dice 3 times and moving in a circle and splitting universes

struct Task
{
	static constexpr int start1 = 7;
	static constexpr int start2 = 3;
	static void run() {
		int score1 = 0;
		int score2 = 0;
		int pos1 = start1-1;
		int pos2 = start2-1;
		int dice = 1;

		int player = 1;
		int rolls = 0;
		while (true) {
			int forward = (dice + 1) * 3 % 10;
			dice+=3;
			rolls+=3;
			if (dice > 100) dice %= 100;
			int& pos = player == 1 ? pos1 : pos2;
			int& score = player == 1 ? score1 : score2;

			pos += forward;
			pos = pos % 10;

			if (pos + score >= 1000) {
				int& otherScore = player == 1 ? score2 : score1;
				std::cout << otherScore << " * " << rolls << " = " << otherScore*rolls << std::endl;
				return;
			}
			score += pos + 1;

			player = player == 1 ? 2 : 1;
		}
	}

	static uint64_t gameCode(int currentPlayer, int pos1, int pos2, int score1, int score2, int diceN) {
		uint64_t result = 0;
		result |= (currentPlayer - 1);
		result <<= 4;
		result |= pos1;
		result <<= 4;
		result |= pos2;
		result <<= 8;
		result |= score1;
		result <<= 8;
		result |= score2;
		result <<= 2;
		result |= diceN;

		return result;
	}
	static void decodeGame(uint64_t gameCode, int& currentPlayer, int& pos1, int& pos2, int& score1, int& score2, int& diceN) {
		diceN = gameCode & 0x3;
		gameCode >>= 2;
		score2 = gameCode & 0xff;
		gameCode >>= 8;
		score1 = gameCode & 0xff;
		gameCode >>= 8;
		pos2 = gameCode & 0xf;
		gameCode >>= 4;
		pos1 = gameCode & 0xf;
		gameCode >>= 4;
		currentPlayer = (gameCode & 0x1) + 1;
	}

	static void split (const uint64_t& code, const uint64_t& n, std::unordered_map<uint64_t, uint64_t>& result, uint64_t& wins1, uint64_t& wins2) {
		int currentPlayer = 0;
		int pos1 = 0;
		int pos2 = 0;
		int score1 = 0;
		int score2 = 0;
		int diceN = 0;
		decodeGame(code, currentPlayer, pos1, pos2, score1, score2, diceN);
		diceN++;
		diceN %= 3;

		const int& pos = currentPlayer == 1 ? pos1 : pos2;
		const int& score = currentPlayer == 1 ? score1 : score2;

		for (int i=1; i<=3; i++) {

			int newPos = (pos + i) % 10;
			int newScore = score;
			int newCurrentPlayer = currentPlayer;

			if (diceN == 0) {
				newScore = (score + newPos + 1);
				newCurrentPlayer = newCurrentPlayer == 1 ? 2 : 1;
			}
			if (newScore >= 21) {
				uint64_t& winner = currentPlayer == 1 ? wins1 : wins2;
				winner+=n;
				continue;
			}

			auto newCode = gameCode(
				newCurrentPlayer,
				currentPlayer == 1 ? newPos : pos1,
				currentPlayer == 2 ? newPos : pos2,
				currentPlayer == 1 ? newScore : score1,
				currentPlayer == 2 ? newScore : score2,
				diceN);
			result[newCode] += n;
		}
	}

	static void runPart2() {
		std::unordered_map<uint64_t, uint64_t> universes1;
		std::unordered_map<uint64_t, uint64_t> universes2;
		uint64_t player1wins = 0;
		uint64_t player2wins = 0;
		universes1.emplace(gameCode(1,start1-1,start2-1,0,0,0), 1);
		auto* src = &universes1;
		auto* dst = &universes2;

		while (!src->empty()) {
			dst->clear();
			for (const auto& [code, n] : *src) {
				split (code, n, *dst, player1wins, player2wins);
			}
			std::swap(src, dst);
		}
		std::cout << (player1wins > player2wins ? player1wins : player2wins) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/