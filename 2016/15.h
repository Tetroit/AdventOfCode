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

//spinning discs

inline int floormod(int a, int b) {
	a %= b;

	if (a < 0) {
		a += b;
	}
	return a;
}
struct Disc {
	int size;
	int offset;

	static int GCD(int a, int b) {
		while (a != b) {
			if (a>b)
				a-=b;
			else
				b-=a;
		}
		return a;
	}
	Disc operator+ (const Disc &d) const {
		Disc res;
		res.size = size / GCD(size, d.size) * d.size;
		int relOffset = floormod(d.offset - offset, d.size);
		//this disc - offset 0
		//d disc offset P
		res.offset = 0;
		while (relOffset != 0) {
			res.offset += size;
			relOffset -= size;
			relOffset = floormod(relOffset, d.size);
			res.offset %= res.size;
		}
		res.offset = floormod(offset + res.offset, res.size);
		return res;
	}
};
struct Task
{
	static inline std::vector<Disc> discs;


	static void run() {

		std::ifstream inputStream("2016/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::regex pattern(R"(Disc #(\d+) has (\d+) positions; at time=0, it is at position (\d+).)");
		while (std::getline(inputStream, line))
		{
			int n, size, offset;
			std::smatch match;
			std::regex_search(line, match, pattern);
			n = std::stoi(match[1].str());
			size = std::stoi(match[2].str());
			offset = floormod(std::stoi(match[3].str()) + n, size);
			discs.emplace_back(size, offset);
		}
		inputStream.close();
		Disc final {1,0};
		for (int i = 0; i < discs.size(); i++) {
			final = discs[i] + final;
		}
		std::cout << final.size - final.offset << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/