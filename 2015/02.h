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

	int area (int a, int b, int c) {
		return 2 * a * b + 2 * b * c + 2 * c * a + std::min(a*b,std::min(b*c,c*a));
	}
	int ribbon (int a, int b, int c) {
		return 2 * std::min(a+b,std::min(b+c,c+a)) + a * b * c;
	}

	void run() {


		std::ifstream inputStream("02.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		int a = 0;
		int b = 0;
		int c = 0;

		char sep;
		int res = 0;
		int ribbonRes = 0;
		while (inputStream >> a)
		{
			inputStream >> sep;
			inputStream >> b;
			inputStream >> sep;
			inputStream >> c;
			res += area(a, b, c);
			ribbonRes += ribbon(a,b,c);
		}
		std::cout << res << "\n";
		std::cout << ribbonRes << "\n";
	}
};
