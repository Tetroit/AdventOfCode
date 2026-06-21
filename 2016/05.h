#pragma once

#include <array>
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

//md5 hashes again AAAAA
struct Task
{
	static inline std::string id = "reyedfim";
	static void run() {
		int i=0;
		int passL = 0;
		std::string pass = "";
		while (true) {
			std::string code = id + std::to_string(i);
			auto hash = QuickDigest5::digestString(code);
			if ((hash[0] == 0) && (hash[1] == 0) && (hash[2] >> 4) == 0) {
				passL++;
				pass += std::format("{:x}", hash[2] & 0xf);
				if (passL == 8)
					break;
			}
			i++;
		}
		std::cout << pass << std::endl;
	}
	static void runPart2() {
		int i=0;
		int passL = 0;
		std::array<uint8_t, 8> pass{};
		uint8_t mask = 0x00;
		while (true) {
			std::string code = id + std::to_string(i);
			auto hash = QuickDigest5::digestString(code);
			if ((hash[0] == 0) && (hash[1] == 0) && (hash[2] >> 4) == 0) {
				uint8_t pos = hash[2] & 0xf;
				if (pos >= 8 || ((mask >> pos) & 1) == 1) {
					i++;
					continue;
				}
				passL++;
				mask |= (1 << pos);
				pass[pos] = hash[3] >> 4;
				for (int p=0; p<8; p++) {
					if (mask & 1 << p) {
						std::cout << std::hex << (int)pass[p];
					}
					else {
						std::cout << '_';
					}
				}
				std::cout << std::endl;
				if (mask == 0xff)
					break;
			}
			i++;
		}
		for (const auto& val : pass) {
			std::cout << std::hex << (int)val;
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/