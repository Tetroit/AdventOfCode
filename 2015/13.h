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
#include "../utils.h"

//Task about guys losing or gaining happiness when seated in a circle
struct Task
{
	typedef std::unordered_map<std::string,std::unordered_map<std::string,int>> Graph;

	static int bestMatchCircle(const Graph& g, std::vector<std::string>* stack) {
		if (stack->size() == g.size()) {
			const std::string& guy1 = stack->back();
			const std::string& guy2 = stack->front();
			return g.at(guy1).at(guy2) + g.at(guy2).at(guy1);
		}
		int max = std::numeric_limits<int>::min();
		for (auto& [neighbour, value] : g.at(stack->back())) {
			if (std::find(stack->begin(), stack->end(), neighbour) != stack->end()) {
				continue;
			}
			stack->push_back(neighbour);
			const std::string& guy1 = stack->back();
			const std::string& guy2 = stack->at(stack->size() - 2);
			int option = g.at(guy1).at(guy2) + g.at(guy2).at(guy1);
			option += bestMatchCircle(g, stack);
			if (option > max) {
				max = option;
			}
			stack->pop_back();
		}
		return max;
	}
	static int bestMatchLine(const Graph& g, std::vector<std::string>* stack) {
		if (stack->size() == g.size()) {
			return 0;
		}
		int max = std::numeric_limits<int>::min();
		for (auto& [guy, neighbourList] : g) {
			if (std::find(stack->begin(), stack->end(), guy) != stack->end()) {
				continue;
			}
			stack->push_back(guy);

			int option = 0;

			if (stack->size() >= 2) {
				const std::string& guy1 = stack->back();
				const std::string& guy2 = stack->at(stack->size() - 2);
				option += g.at(guy1).at(guy2) + g.at(guy2).at(guy1);
			}
			if (stack->size() == g.size()) {
				stack->pop_back();
				return option;
			}
			int inner = bestMatchLine(g, stack);
			option += inner;
			if (option > max) {
				max = option;
			}
			stack->pop_back();
		}
		return max;
	}
	static void run() {

		std::ifstream inputStream("13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		Graph relationships;
		std::string line;
		std::regex pattern(R"((\w+) would (gain|lose) (\d+) happiness units by sitting next to (\w+).)");
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (regex_match(line, match, pattern)) {
				std::string name1 = match[1].str();
				std::string effect = match[2].str();
				int points = std::stoi(match[3].str());
				std::string name2 = match[4].str();
				if (effect == "lose") {
					points = -points;
				}
				relationships[name1][name2] = points;
			}
		}
		inputStream.close();
		std::vector<std::string> stack;
		stack.reserve(relationships.size());
		stack.push_back(relationships.begin()->first);
		int res = bestMatchCircle(relationships, &stack);

		std::cout << res << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		Graph relationships;
		std::string line;
		std::regex pattern(R"((\w+) would (gain|lose) (\d+) happiness units by sitting next to (\w+).)");
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (regex_match(line, match, pattern)) {
				std::string name1 = match[1].str();
				std::string effect = match[2].str();
				int points = std::stoi(match[3].str());
				std::string name2 = match[4].str();
				if (effect == "lose") {
					points = -points;
				}
				relationships[name1][name2] = points;
			}
		}
		inputStream.close();
		std::vector<std::string> stack;
		stack.reserve(relationships.size());
		int res = bestMatchLine(relationships, &stack);

		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/