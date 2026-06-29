#pragma once

#include <algorithm>
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

//generating keys with salt

struct Task
{
	static constexpr int nKey = 64;
	static constexpr std::string salt = "zpqevtbw";
	static inline std::vector<std::pair<uint8_t, int>> keyCandidates;
	static inline std::vector<int> keys;

	static std::vector<uint8_t> nmd5(std::string salt, int repeats) {
		constexpr char hexDigits[] = "0123456789abcdef";
		std::vector<uint8_t> md5;
		for (int i=0; i<repeats; i++) {
			md5 = QuickDigest5::digestString(salt);
			for (int j=0; j<md5.size()*2; j++) {
				const auto& byte = md5[j/2];
				uint8_t dig = j & 1 ? byte & 15 : byte >> 4;
				salt.resize(md5.size()*2);
				salt[j] = hexDigits[dig];
			}
		}
		return md5;
	}
	static std::pair<uint8_t, bool> findRepeats(const std::vector<uint8_t>& md5, int n) {
		uint8_t key1, key2;
		int cnt = 1;
		key1 = md5[0] >> 4;
		for (int i=1; i<md5.size()*2; i++) {
			const auto& byte = md5[i>>1];
			key2 = (i & 1) ? byte & 15 : byte >> 4;
			if (key1 == key2) cnt++;
			else cnt = 1;
			key1 = key2;
			if (cnt == n) return {key2, true};
		}
		return {0, false};
	}
	static bool findRepeats(const std::vector<uint8_t>& md5, uint8_t key, int n) {

		int cnt = 0;
		for (int i=0; i<md5.size()*2; i++) {
			const auto& byte = md5[i>>1];
			uint8_t comp = (i & 1) ? byte & 15 : byte >> 4;
			if (comp == key) cnt++;
			else cnt = 0;
			if (cnt == n) return true;
		}
		return false;
	}

	static void run() {
		int keyCnt = 0;
		int suff = 0;
		int countDown = -1;
		while (true) {
			auto md5 = QuickDigest5::digestString(salt + std::to_string(suff));
			int firstValid = -1;
			for (int i=0; i<keyCandidates.size(); i++) {
				const auto& [hex, id] = keyCandidates.at(i);
				if (suff - id > 1000)
					continue;
				if (firstValid == -1)
					firstValid = i;
				if (findRepeats(md5, hex, 5)) {
					keyCnt++;
					std::cout << id << " found at " << suff << std::endl;
					keys.emplace_back(id);
					keyCandidates.erase(keyCandidates.begin()+i);
					if (keyCnt == nKey) {
						std::cout << "start countdown" << std::endl;
						countDown = 1000;
					}
					i--;
				}
			}
			if (firstValid > 0)
				keyCandidates.erase(keyCandidates.begin(), keyCandidates.begin() + firstValid);
			auto [hex, success] = findRepeats(md5, 3);
			if (success && countDown == -1) {
				keyCandidates.emplace_back(hex, suff);
			}
			suff++;
			if (countDown > 0) {
				countDown--;
			}
			if (countDown == 0) {
				break;
			}
		}
		std::ranges::sort(keys, std::less<int>());
		std::cout << keys[nKey-1] << std::endl;
	}
	static void runPart2() {
		int keyCnt = 0;
		int suff = 0;
		int countDown = -1;
		keyCandidates.clear();
		keys.clear();
		while (true) {
			auto md5 = nmd5(salt + std::to_string(suff), 2017);
			int firstValid = -1;
			for (int i=0; i<keyCandidates.size(); i++) {
				const auto& [hex, id] = keyCandidates.at(i);
				if (suff - id > 1000)
					continue;
				if (firstValid == -1)
					firstValid = i;
				if (findRepeats(md5, hex, 5)) {
					keyCnt++;
					std::cout << id << " found at " << suff << std::endl;
					keys.emplace_back(id);
					keyCandidates.erase(keyCandidates.begin()+i);
					if (keyCnt == nKey) {
						std::cout << "start countdown" << std::endl;
						countDown = 1000;
					}
					i--;
				}
			}
			if (firstValid > 0)
				keyCandidates.erase(keyCandidates.begin(), keyCandidates.begin() + firstValid);
			auto [hex, success] = findRepeats(md5, 3);
			if (success && countDown == -1) {
				keyCandidates.emplace_back(hex, suff);
			}
			suff++;
			if (countDown > 0) {
				countDown--;
			}
			if (countDown == 0) {
				break;
			}
		}
		std::ranges::sort(keys, std::less<int>());
		std::cout << keys[nKey-1] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/