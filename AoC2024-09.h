#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{
	int countTotal(const std::vector<int>& input) {
		int res = 0;
		for (int i = 0; i < input.size(); i+=2) {
			res += input[i];
		}
		return res;
	}
	void copyGaps(const std::vector<int>& map, std::vector<int>& output) {
		for (int i=1 ; i<map.size(); i+=2) {
			output.push_back(map[i]);
		}
	}
	void tryMove (const std::vector<int>& map, int endId, std::map<int, std::pair<int, int>>& reg, std::vector<int>& gaps) {
		int space = 0;
		int rest = 0;
		int size = map[endId];
		int id = endId/2;
		for (int i=1 ; i < endId; i+=2) {
			space += map[i-1];
			int& gap = gaps[i/2];
			space += map[i] - gap;
			rest = gap;
			if (gap >= size) {
				gap -= size;
				reg.try_emplace(space, std::make_pair(id, size));
				return;
			}
			space += rest;
		}
		reg.try_emplace(space, std::make_pair(id, size));
	}
	bool validate(std::map<int, std::pair<int, int>>& reg) {
		int last = -1;
		for (auto& val : reg) {
			int first = val.first;
			if (last > first)
				return false;
			last = first + val.second.second;
		}
		return true;
	}

	unsigned long long calculate (const std::map<int, std::pair<int, int>>& reg) {
		unsigned long long res = 0;
		for (auto& val : reg) {

			long long id = val.second.first;
			long long size = val.second.second;
			long long pos = val.first;
			long long sectionScore = id * ((size - 1) * size / 2 + pos * size);

			res += sectionScore;
		}
		return res;
	}
	void run() {

		std::ifstream inputStream("input2024-09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		char ch;
		std::vector<int> map;
		while (inputStream >> ch)
		{
			map.push_back(ch - '0');

		}

		int size = map.size();
		if (size % 2 == 0) {
			map.pop_back();
			size--;
		}
		int startId = 0;
		int endId = size/2;
		int spaceId = 0;
		int forwardCnt = map[0];
		int backwardCnt = map[size - 1];
		int freeSpaces = map[1];
		int memId = 0;

		bool countForward = true;
		int total = countTotal(map);
		unsigned long long sum = 0;

		//task 1
		while (true){

			if (memId >= total) {
				break;
			}
			if (countForward) {

				//file ended
				if  (forwardCnt == 0) {
					startId++;
					if (startId * 2 < map.size()) {
						forwardCnt = map[startId*2];
						countForward = false;
						continue;
					}
				}
			}
			else {
				//file started
				if (freeSpaces == 0) {
					if ((spaceId + 1) * 2 + 1< map.size()) {
						spaceId++;
						freeSpaces = map[spaceId * 2 + 1];
						countForward = true;
						continue;
					}
				}
				if (backwardCnt == 0) {
					if (endId > 0) {
						endId --;
						backwardCnt = map[endId*2];
					}
					else {
						countForward = true;
						continue;
					}

				}
			}

			//add to sum
			if (countForward) {
				forwardCnt--;
				sum += startId * memId;
			}
			else {
				backwardCnt--;
				freeSpaces--;
				sum += endId * memId;
			}
			memId++;
		}
		//task 2
		std::map<int, std::pair<int, int>> reg;
		std::vector<int> gaps;

		copyGaps(map, gaps);

		for (int i = map.size() - 1; i >= 0; i-=2) {
			tryMove(map, i, reg, gaps);
		}

		unsigned long long sum2 = calculate(reg);

		std::cout << validate(reg) << '\n';

		std::cout << sum << '\n';
		std::cout << sum2 << '\n';

		//1599576248314
	}
};
