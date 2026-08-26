#pragma once

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
#include <array>
#include "utils.h"

//using extending sequence to generate a new number


struct Task
{
	static inline constexpr std::array<int, 4> salt = {0, 1, 0, -1};
	static void run() {

		std::ifstream inputStream("2019/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::vector<int> seq;
		std::vector<int> seq2;
		char ch;
		while (inputStream >> ch)
		{
			seq.push_back(ch-'0');
		}
		inputStream.close();
		auto* src = &seq;
		auto* dst = &seq2;

		static constexpr int repeats = 1;
		for (int r = 0; r < repeats; r++) {
			dst->clear();
			dst->resize(src->size());
			for (int i=0; i<dst->size(); i++) {
				auto& digit = dst->at(i);
				int loop = 4*(i+1);
				for (int l = i; l<i+i+1; l++) {
					for (int j = l; j < src->size(); j+=loop) {
						digit += src->at(j);
					}
				}
				for (int l = i*3+2; l<i*4+3; l++) {
					for (int j = l; j < src->size(); j+=loop) {
						digit -= src->at(j);
					}
				}
				if (digit < 0) digit = -digit;
				digit %= 10;
			}
			std::swap(dst, src);
		}
		for (int i : *src) {
			std::cout << i;
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2019/16.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::vector<int> seq;
		char ch;
		while (inputStream >> ch)
		{
			seq.push_back(ch-'0');
		}
		int offset = 0;
		for (int i=0; i<7; i++) {
			offset += Utils::powInt(10,6-i) * seq.at(i);
		}
		int seqSize = seq.size();
		inputStream.close();
		std::vector<int> lastSeq;
		std::vector<int> lastSeq2;
		for (int i=offset; i<seqSize * 10000; i++) {
			lastSeq.push_back(seq.at(i%seqSize));
		}

		auto* src = &lastSeq;
		auto* dst = &lastSeq2;
		static constexpr int repeats = 100;
		for (int r = 0; r < repeats; r++) {
			dst->clear();
			dst->resize(src->size());
			int acc = 0;
			for (int i=src->size()-1; i>=0; i--) {
				acc+=src->at(i);
				dst->at(i) = acc%10;
			}
			std::swap(dst, src);
		}
		for (int i=0; i<8; i++) {
			std::cout << src->at(i);
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
+0-0+0-0+0-0+0-0+0-0+0-0+0-0+0-0
0++00--00++00--00++00--00++00--0
00+++000---000+++000---000+++000
000++++0000----0000++++0000----0
0000+++++00000-----00000+++++000
00000++++++000000------000000+++
000000+++++++0000000-------00000
0000000++++++++00000000--------0
...
0000000000000000000000000000000+

1 0 -1 0 1 0 -1 0 1 0
0 1 1 0 0 -1 -1 0 0 1
0 0 1 1 1 0 0 0 -1 -1 -1
0 0 0 1 1 1 1 0 0 0
0 0 0 0 1 1 1 1 1 0
-------------------
0 0 0 0 0 1 1 1 1 1
0 0 0 0 0 0 1 1 1 1
0 0 0 0 0 0 0 1 1 1
0 0 0 0 0 0 0 0 1 1
0 0 0 0 0 0 0 0 0 1

the matrix is basically triangular
and since we only need digits from second half it
they are basically just equal to the sum of all digits coming after that

consider 3 last patterns
...000111
...000011
...000001
they all differ by just 1 digit
we can calculate all numbers in 1 loop starting with the last element in the new sequence
a_n' = a_n
then for each previous element
a_(i-1)' = a_i' + a_(i-1)

*/

