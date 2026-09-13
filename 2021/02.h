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

//traversing map

struct Task
{
	static void run() {

		std::ifstream inputStream("2021/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string command;
		int x = 0, y = 0;
		int val;
		while (inputStream >> command >> val)
		{
			if (command == "forward") x+=val;
			else if (command == "backward") x-=val;
			else if (command == "up") y-=val;
			else if (command == "down") y+=val;
		}
		inputStream.close();
		std::cout << x * y << '\n';
	}
	static void runPart2() {
		std::ifstream inputStream("2021/02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string command;
		int x = 0, y = 0;
		int aim = 0;
		int val;
		while (inputStream >> command >> val)
		{
			if (command == "forward") {x+=val; y+=aim * val;}
			else if (command == "backward") {x-=val; y-=aim * val;}
			else if (command == "up") aim-=val;
			else if (command == "down") aim+=val;
		}
		inputStream.close();
		std::cout << x * y << '\n';
	}
};

//-------------- NOTES AREA ----------------
/*

*/