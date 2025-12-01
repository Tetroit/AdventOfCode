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

//TODO: put a small description of the task
struct Task
{
	static void run() {

///TODO: CHANGE THE DAMN FILE NAME BEFORE YOU FORGET

		std::ifstream inputStream("input20XX-00.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		while (std::getline(inputStream, line))
		{

		}
		inputStream.close();
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/