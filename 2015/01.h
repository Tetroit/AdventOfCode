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
#include "../utils.h"

struct Task
{
	void run() {


		std::ifstream inputStream("01.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		char symb;
		int cnt = 0;
		int pos = 0;
		bool entered = false;
		int enterPos = 0;
		while (inputStream >> symb)
		{
			pos ++;
			if (symb == '(')
				cnt++;
			else if (symb == ')')
				cnt--;

			if (cnt < 0 && !entered) {
				entered = true;
				enterPos = pos;
			}
		}
		std::cout << cnt << "\n";
		std::cout << enterPos << "\n";
	}
};
