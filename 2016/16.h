#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//filling array with bits and reducing to get checksum

struct Task
{
	static constexpr size_t size = 272;
	static constexpr size_t size2 = 35651584;
	static inline const std::string startLine = "01111010110010011";

	template <size_t n>
	static void shrink(std::bitset<n>& src, size_t& arrSize) {
		for (size_t i=0; i*2+1<arrSize; i++) {
			src[i] = (src[2*i] == src[2*i+1]);
		}
		arrSize>>=1;
	}

	template <size_t n>
	static void expand(std::bitset<n>& disk, size_t& len) {
		while (len < n) {
			disk[len] = false;
			for (size_t i=len-1; i < len; i--) {
				size_t dst = 2 * len - i;
				if (dst >= n) {
					len = n;
					return;
				}
				disk[dst] = !disk[i];
			}
			len = len * 2 + 1;
		}
	}

	// template <size_t n>
	// static auto shrink_recursive(std::bitset<n> src) {
	// 	if constexpr (n % 2 == 0) {
	// 		return shrink_recursive <n/2>(shrink<n/2>(src));
	// 	}
	// 	else {
	// 		return src;
	// 	}
	// }
	static void run() {
		auto disk = std::make_unique<std::bitset<size>>();
		size_t len = startLine.length();
		for (size_t i=0; i<len; i++) {
			(*disk)[i] = (startLine[i] == '1');
		}
		expand<size>(*disk, len);
		while (len % 2 == 0) {
			shrink<size>(*disk, len);
		}
		for (size_t i=0; i<len; i++) {
			std::cout << (*disk)[i];
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		auto disk = std::make_unique<std::bitset<size2>>();
		size_t len = startLine.length();
		for (size_t i=0; i<len; i++) {
			(*disk)[i] = (startLine[i] == '1');
		}
		expand<size2>(*disk, len);
		while (len % 2 == 0) {
			shrink<size2>(*disk, len);
		}
		for (size_t i=0; i<len; i++) {
			std::cout << (*disk)[i];
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/