#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "../utils.h"

struct Task
{
	bool can_concat(long long res, long long b, long long& a)
	{
		long long copy = b;
		long long biggerRound = 1;
		while (copy > 0)
		{
			copy /= 10;
			biggerRound *= 10;
		}
		if ((res % biggerRound) == b)
		{
			a = res / biggerRound;
			return true;
		}
		return false;
	}
	bool calculate(long long res, const std::vector<long long>::const_iterator& begin, const std::vector<long long>::const_iterator& end)
	{
		if (begin == end)
		{
			return res == (*begin);
		}

		long long subRes = 0;
		if (can_concat(res, (*end), subRes))
		{
			if (calculate(subRes, begin, end - 1)) {
				return true;
			}
		}

		if (res % (*end) == 0) 
		{
			if (calculate(res / (*end), begin, end - 1)) {
				return true;
			}
		}

		return calculate(res - (*end), begin, end - 1);

	}
	bool calculate(const std::vector<long long>& arr) {
		return calculate(arr[0], arr.cbegin() + 1, arr.cend() - 1);
	}

	void run() {

		std::string line;
		std::ifstream inputStream("input2024-07.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::vector<std::vector<long long>> equations;
		
		long long cnt = 0;

		while (std::getline(inputStream, line))
		{
			int ddotId = line.find(':');
			std::string resStr = line.substr(0, ddotId);

			long long res = 0;
			res = std::stoll(resStr);
			equations.emplace_back();
			auto& equation = equations.back();
			equation.push_back(res);

			line = line.substr(ddotId + 2, line.length() - ddotId - 1);
			auto words = split(line, ' ');
			for (auto word : words)
			{
				equation.push_back(std::stoi(word));
			}
		}

		for (auto& eq : equations)
		{
			if (calculate(eq))
				cnt+=eq[0];
		}
		std::cout << '\n' << cnt;
	}
};
