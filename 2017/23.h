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

//TODO: put a small description of the task
struct Task
{
	static void run() {

///TODO: CHANGE THE DAMN FILE NAME BEFORE YOU FORGET

		std::ifstream inputStream("20XX/input/XX.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
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