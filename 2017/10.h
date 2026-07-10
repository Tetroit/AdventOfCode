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

//twisting loop and making knots

struct Task
{
	template <typename T>
	static void reverse(std::vector<T>& vec, int start, int size) {
		int end = (start + size - 1) % (int)vec.size();
		int l = start;
		int r = end;
		for (int i = 0; i < size; i+=2) {
			std::swap(vec[r], vec[l]);
			r--;
			if (r < 0)
				r+= (int)vec.size();
			l++;
			if (l >= vec.size())
				l -= (int)vec.size();
		}
	}
	static void run() {

		std::ifstream inputStream("2017/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char shit;
		int num;
		std::vector<int> loops;
		while (inputStream >> num)
		{
			loops.push_back(num);
			inputStream >> shit;
		}
		inputStream.close();

		std::vector<int> ring (256, 0);
		for (int i=0; i<ring.size(); i++) {
			ring[i] = i;
		}
		int currentPos = 0;
		int currentStride = 0;
		for (int loop : loops) {
			reverse(ring, currentPos, loop);
			currentPos = (currentPos + loop + currentStride) % (int)ring.size();
			currentStride++;
		}
		std::cout << ring[0] * ring[1] << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/10.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char shit;
		char num;
		std::vector<char> loops;
		while (inputStream >> num)
		{
			loops.push_back(num);
		}
		loops.insert(loops.end(), {17, 31, 73, 47, 23});
		inputStream.close();

		std::vector<char> ring (256, 0);
		for (int i=0; i<ring.size(); i++) {
			ring[i] = i;
		}
		int currentPos = 0;
		int currentStride = 0;
		for (int i = 0; i<64; i++) {
			for (char loop : loops) {
				reverse(ring, currentPos, loop);
				currentPos = (currentPos + loop + currentStride) % (int)ring.size();
				currentStride++;
			}
		}
		std::vector<char> dense (16, 0);
		for (int i=0; i<16; i++) {
			char hash = 0;
			for (int j=0; j<16; j++) {
				hash ^= ring[i*16+j];
			}
			dense[i] = hash;
		}
		for (int i=0; i<16; i++) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)static_cast<unsigned char>(dense[i]);
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/