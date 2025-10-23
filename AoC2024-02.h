#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include "utils.h"

struct Task
{
	enum Status {
		INCREASING,
		DECREASING,
		UNKNOWN
	};

	bool check_neighbours(int& num1, int& num2, Status status)
	{
		int diff = std::abs(num2 - num1);
		if (status == INCREASING && num2 < num1)
			return false;
		if (status == DECREASING && num2 > num1)
			return false;

		if (diff == 0 || diff > 3)
			return false;

		return true;
	}

	bool check(std::vector<int>& vec)
	{

		Status status = DECREASING;
		if (vec[1] > vec[0])
			status = INCREASING;
		for (int i = 0; i < vec.size() - 1; i++)
		{
			if (!check_neighbours(vec[i], vec[i+1], status))
				return false;
		}
		return true;

	}
	bool check_advanced(std::vector<int>& vec) {

		for (int i = 0; i < vec.size(); i++)
		{

			Status status = DECREASING;

			switch (i) {
			case 0:

				if (vec[2] > vec[1])
					status = INCREASING;
				break;

			case 1:
				if (vec[2] > vec[0])
					status = INCREASING;
				break;

			default:
				if (vec[1] > vec[0])
					status = INCREASING;
				break;
			}

			bool possible = true;
			for (int j = (i==0) ? 1 : 0; j < vec.size() - 1; j++)
			{
				if (j + 1 == i && i != vec.size() - 1)
				{
					j++;
					if (!check_neighbours(vec[i - 1], vec[i + 1], status))
						possible = false;
				}
				else if (j + 1 == i && i == vec.size() - 1)
				{
					j++;
					continue;
				}
				else {
					if (!check_neighbours(vec[j], vec[j + 1], status))
						possible = false;
				}
			}
			if (possible)
				return true;
		}
		return false;
	}
	void run() {

		std::ifstream inputStream("input2024-02.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string report;
		int cnt = 0;
		int reps = 0;
		while (std::getline(inputStream, report))
		{
			reps++;
			auto words = split(report, ' ');
			std::vector<int> parsed;
			if (words.size() <= 1)
			{
				cnt++;
				continue;
			}
			for (auto& word : words)
			{
				int num = std::stoi(word);
				parsed.push_back(num);
			}
			if (check(parsed))
			{
				cnt++;
				continue;
			}

			//part 2

			if (parsed.size() == 2)
			{
				cnt++;
				continue;
			}
			if (check_advanced(parsed)) {
				cnt++;
				continue;
			}
		}
		std::cout << cnt << "/" << reps << '\n';
	}
};