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

//Checking passwords

struct Task
{
	static void run() {

		std::ifstream inputStream("2020/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int nValid = 0;
		int nValid2 = 0;
		while (std::getline(inputStream, line))
		{
			std::istringstream iss(line);
			int min, max;
			char key;
			std::string str;
			iss >> min;
			iss.ignore(1);
			iss >> max >> key;
			iss.ignore(2);
			iss >> str;

			int cnt = 0;
			for (auto c : str) {
				if (c == key) cnt++;
			}
			if (cnt >= min && cnt <= max) nValid++;

			//part 2
			if ((str[min-1] == key) != (str[max-1] == key)) nValid2++;
		}
		inputStream.close();
		std::cout << nValid << std::endl;
		std::cout << nValid2 << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/