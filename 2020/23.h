#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Shuffling 10 cups
struct Task
{
	//this is a doodoo ass sloppy solution that is as efficient as potato
	//I made a version backed by std::list that looked so horrible it shan't be laid upon thy eyes :skull:
	static void run() {

		std::vector<int> seq {1,9,3,4,6,7,2,5,8};
		constexpr int repeats = 100;
		int ptr = 0;
		for (int repeat = 0; repeat < repeats; repeat++) {
			int start = (ptr+1)%seq.size();
			int target = seq[ptr] - 1;
			if (target==0) target = seq.size();

			for (int i=0; i<seq.size(); i++) {
				if (i == ptr) std::cout << '(';
				std::cout << seq[i];
				if (i==ptr) std::cout << ')';
			}
			std::cout << std::endl;

			std::vector<int> temp;
			for (int i=0; i<3; i++) {
				temp.push_back(seq[(start+i)%seq.size()]);
			}
			for (int i=0; i<3; i++) {
				if (target == temp[i]) {
					i = -1;
					target--;
					if (target==0) target = seq.size();
				}
			}
			for (int i=0; i<3; i++) {
				int pos = start;
				if (start >= seq.size()) {
					pos = 0;
				}
				if (pos < ptr) ptr--;
				seq.erase(seq.begin()+pos);
			}
			auto dst = std::find(seq.begin(), seq.end(), target)+1;
			if (dst - seq.begin() <= ptr) ptr+=3;
			seq.insert(dst, temp.begin(), temp.end());
			ptr = (ptr+1)%seq.size();

			for (auto t : temp) std::cout << t;
			std::cout << std::endl;
			std::cout << target << std::endl;
		}
		auto loc = std::find(seq.begin(), seq.end(), 1);
		for (int i=1; i<seq.size(); i++) {
			loc++;
			if (loc == seq.end()) loc = seq.begin();
			std::cout << *loc;
		}
		std::cout << std::endl;
	}
	static void runPart2() {
		std::vector<int> startSeq {1,9,3,4,6,7,2,5,8};
		std::vector<int> next(1000001);
		for (int i=1; i<=1000000; i++) {
			int target = (i < 10) ? startSeq[i-1] : i;
			int n = i == 1000000 ? 1 : i+1;
			next[target] = (n < 10) ? startSeq[n-1] : n;
		}
		constexpr int repeats = 10000000;

		int src = startSeq[0];
		int c1, c2, c3, dst;

		for (int repeat = 0; repeat < repeats; repeat++) {
			c1 = next[src];
			c2 = next[c1];
			c3 = next[c2];

			dst = src;
			bool taken = true;
			while (taken) {
				dst--;
				taken = c1 == dst || c2 == dst || c3 == dst;
				if (dst == 0) dst = next.size()-1;
			}
			next[src] = next[c3];
			next[c3] = next[dst];
			next[dst] = c1;

			src = next[src];
		}
		std::cout << (long long)next[1] * (long long)next[next[1]] << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

	basically since we have a set of consecutive numbers we can store 1 array,
	where number at i stores the label clockwise from label i

*/