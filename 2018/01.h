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
#include "utils.h"

//putting all changes together
struct Task
{
	static void run() {

		std::ifstream inputStream("2018/01.txt");

		int c;
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int cnt = 0;
		while (inputStream >> c)
		{
			cnt+=c;
		}
		inputStream.close();
		std::cout << cnt << std::endl;
	}
	static void runPart2() {

		std::ifstream inputStream("2018/01.txt");

		int c;
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::vector<int> diffs;
		while (inputStream >> c)
		{
			diffs.push_back(c);
		}
		inputStream.close();
		int i=0;
		int cnt = 0;
		std::unordered_set<int> list{cnt};
		while (true) {
			cnt+=diffs[i];
			if (list.contains(cnt)) {
				std::cout << cnt << std::endl;
				break;
			}
			list.emplace(cnt);
			i++;
			i%=diffs.size();
		}
		list.clear();
	}
};

//-------------- NOTES AREA ----------------
/*

*/