#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <filesystem>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"
#include "DynamicBitset.h"

//Unpacking binary

struct Task
{
	static inline DynamicBitset bitset;

	static uint64_t readPacket(const DynamicBitset& arr, int& ptr, int& versionSum) {
		uint32_t version = arr.slice(ptr, 3);
		versionSum+=version;
		ptr+=3;
		uint32_t type = arr.slice(ptr, 3);
		ptr+=3;

		if (type == 4) {
			uint64_t num = 0;
			bool last = false;
			do {
				last = !arr[ptr++];
				num = (num << 4) | arr.slice(ptr, 4);
				ptr+=4;
			} while (!last);
			return num;
		}
		uint32_t size = 0;
		std::vector<uint64_t> vals;
		if (arr[ptr++] == 1) {
			size = arr.slice(ptr, 11);
			ptr+=11;
			for (int i=0; i<size; i++) {
				vals.emplace_back(readPacket(arr, ptr, versionSum));
			}
		}
		else {
			size = arr.slice(ptr, 15);
			ptr+=15;
			uint32_t last = ptr + size - 1;
			while (ptr < last) {
				vals.emplace_back(readPacket(arr, ptr, versionSum));
			}
		}
		uint64_t num = 0;
		switch (type) {
			case 0: {
				for (uint64_t val : vals) num+=val;
				break;
			}
			case 1: {
				num = 1;
				for (uint64_t val : vals) num*=val;
				break;
			}
			case 2: {
				num = UINT64_MAX;
				for (uint64_t val : vals) if (val < num) num = val;
				break;
			}
			case 3: {
				for (uint64_t val : vals) if (val > num) num = val;
				break;
			}
			case 5: {
				num = vals[0] > vals[1];
				break;
			}
			case 6: {
				num = vals[0] < vals[1];
				break;
			}
			case 7: {
				num = vals[0] == vals[1];
				break;
			}
			default: {}
		}
		return num;
	}
	static void run() {

		std::ifstream inputStream("2021/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char ch;
		while (inputStream >> ch)
		{
			bitset.pushCharHex(ch);
		}
		inputStream.close();
		int versionSum = 0;
		int ptr = 0;
		readPacket(bitset, ptr, versionSum);
		std::cout << versionSum << std::endl;
	}
	static void runPart2() {
		int versionSum = 0;
		int ptr = 0;
		std::cout << readPacket(bitset, ptr, versionSum) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/