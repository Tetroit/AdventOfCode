#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "utils.h"

struct Task
{
	void parse_rule(std::unordered_map<int, std::vector<int>>& rules, const std::string& string)
	{
		auto words = split(string, '|');

		int num1 = std::stoi(words[0]);
		int num2 = std::stoi(words[1]);

		rules[num1].push_back(num2);
	}
	void parse_update(std::vector<std::vector<int>>& updates, const std::string& string)
	{
		auto words = split(string, ',');
		updates.push_back(std::vector<int>());

		for (int i = 0; i < words.size(); i++)
		{
			updates.back().push_back(std::stoi(words[i]));
		}
	}
	int get_middle(const std::vector<int>& vec) {
		return vec[(vec.size() - 1)/2];
	}

	bool check(const std::vector<int>& vec, std::unordered_map<int, std::vector<int>>& rules) {
		for (int high = 1; high < vec.size(); high++)
		{
			auto& page = vec[high];
			for (int low = high - 1; low >= 0; low--)
			{
				if (rules.find(page) == rules.end())
					continue;
				auto& lowPage = vec[low];
				auto& container = rules[page];
				if (std::find(container.begin(), container.end(), lowPage) != container.end()) {
					std::cout << "for the line: " << '\n';
					for (int i : vec)
					{
						std::cout << i << ',';
					}
					std::cout << '\n';
					std::cout << lowPage << " is not allowed before " << page << "\n\n";
					return false;
				}
			}
		}
		return true;
	}

	bool check_fix(std::vector<int>& vec, std::unordered_map<int, std::vector<int>>& rules)
	{
		bool success = true;
		for (int high = 1; high < vec.size(); high++)
		{
			auto& page = vec[high];
			if (rules.find(page) == rules.end())
				continue;

			for (int low = high - 1; low >= 0; low--)
			{
				auto& lowPage = vec[low];
				auto& container = rules[page];
				if (std::find(container.begin(), container.end(), lowPage) != container.end()) {
					std::swap(page, lowPage);
					success = false;
					//check everything again from the place where changes were made
					high = low-1;
					break;
				}
			}
		}
		return success;
	}
	void run() {

		std::ifstream inputStream("input2024-05.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		bool secondSection = false;

		std::string line;
		std::unordered_map<int, std::vector<int>> rules;
		std::vector<std::vector<int>> updates;
		while (std::getline(inputStream, line))
		{
			if (line.size() == 0)
				secondSection = true;
			else if (secondSection)
			{
				parse_update(updates, line);
			}
			else
			{
				parse_rule(rules, line);
			}
		}

		int sum = 0;
		int fixedSum = 0;
		for (auto& vec : updates) {
			/*if (check(vec, rules))
				sum += get_middle(vec);*/

			//part 2

			if (!check_fix(vec, rules))
				fixedSum += get_middle(vec);
			for (int i : vec)
			{
				std::cout << i << ',';
			}
			std::cout << '\n';
		}
		std::cout << fixedSum << '\n';
	}
};
