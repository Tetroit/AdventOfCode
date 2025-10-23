#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{
	void run() {

///TODO: future me, CHANGE THE DAMN FILE NAME BEFORE YOU FORGET

		std::ifstream inputStream("input20XX-00.txt");
///TODO: ----------------------------------------------------------

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		while (std::getline(inputStream, line))
		{

		}
	}
};
