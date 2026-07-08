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

//captcha: match with next digit and sum
struct Task
{
	static void run() {

		std::ifstream inputStream("2017/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{

		}
		inputStream.close();
		int cnt = 0;
		for (int i=0; i<line.length()-1; i++) {
			if (line[i] == line[i+1])
				cnt+=line[i] - '0';
		}
		if (line[line.length() - 1] == line[0])
			cnt+=line[0] - '0';
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2017/01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{

		}
		inputStream.close();
		int cnt = 0;
		int stride = line.length()/2;
		for (int i=0; i<line.length()-1; i++) {
			if (line[i] == line[(i+stride)%line.length()])
				cnt+=line[i] - '0';
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/