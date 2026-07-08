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

//difference between min and max in each row

struct Task
{
	static void run() {

		std::ifstream inputStream("2017/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		int cnt = 0;
		while (inputStream.peek() != EOF)
		{
			int min = std::numeric_limits<int>::max();
			int max = std::numeric_limits<int>::min();
			while (inputStream.peek() != '\n' && inputStream.peek() != EOF) {
				inputStream >> num;
				if (num < min)
					min = num;
				if (num > max)
					max = num;
			}
			cnt += max - min;
			if (inputStream.peek() == '\n')
				inputStream.get();
		}
		inputStream.close();
		std::cout << cnt << '\n';
	}
	static void runPart2() {
		std::ifstream inputStream("2017/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int num;
		int cnt = 0;
		while (inputStream.peek() != EOF)
		{
			std::vector<int> vec;
			while (inputStream.peek() != '\n' && inputStream.peek() != EOF) {
				inputStream >> num;
				vec.push_back(num);
			}
			for (int i=0; i<vec.size(); i++) {
				for (int j=0; j<vec.size(); j++) {
					if (i == j) continue;
					if (vec[i] % vec[j] == 0)
						cnt+= vec[i] / vec[j];
				}
			}
			if (inputStream.peek() == '\n')
				inputStream.get();
		}
		inputStream.close();
		std::cout << cnt << '\n';
	}
};

//-------------- NOTES AREA ----------------
/*

*/