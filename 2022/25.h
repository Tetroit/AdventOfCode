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

//converting SNAFU to decimal
struct Task
{
	static void run() {

		std::ifstream inputStream("2022/25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		long long sum = 0;
		while (std::getline(inputStream, line))
		{
			long long num = 0;
			for (char i : line) {
				num*=5;
				if (i == '1') {
					num++;
				}
				else if (i == '2') {
					num+=2;
				}
				else if (i == '-') {
					num--;
				}
				else if (i == '=') {
					num-=2;
				}
			}
			sum += num;
		}
		inputStream.close();
		std::cout << sum << std::endl;

		std::string res;
		int carry = 0;
		while (sum != 0) {
			if (sum % 5 == 0)
				res.insert(res.begin(), '0');
			else if (sum % 5 == 1)
				res.insert(res.begin(), '1');
			else if (sum % 5 == 2)
				res.insert(res.begin(), '2');
			else if (sum % 5 == 3) {
				carry = 1;
				res.insert(res.begin(), '=');
			}
			else if (sum % 5 == 4) {
				carry = 1;
				res.insert(res.begin(), '-');
			}
			sum = sum / 5 + carry;
			carry = 0;
		}
		std::cout << res << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/