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
#include "utils.h"

//Task about mixing ingredients and cookies


struct Task
{
	struct Stats {
	public:
		int capacity;
		int durability;
		int flavor;
		int texture;
		int calories;

		Stats operator+ (const Stats& other) const {
			return Stats{
				capacity + other.capacity,
				durability + other.durability,
				flavor + other.flavor,
				texture + other.texture,
				calories + other.calories
			};
		}
		Stats operator* (const int& amount) const {
			return Stats{
				capacity * amount,
				durability * amount,
				flavor * amount,
				texture * amount,
				calories * amount,
			};
		}

		long long score() {
			if (capacity < 0 || durability < 0 || flavor < 0 || texture < 0) return 0;
			return
				(long long)capacity *
				(long long)durability *
				(long long)flavor *
				(long long)texture;
		}
	};
	inline static std::vector<Stats> ingredients;

	//Executes any function of type Fn for every proportion
	//Fn can look like (as in this task)
	//void foo(const std::vector<Stats>& stats, const std::vector<int>& proportions){}
	template <typename Fn>
	static void iterateProportions(
		const std::vector<Stats>& stats,
		int N, int total, Fn&& fn) {

		std::vector<int> proportions;
		proportions.reserve(stats.size());
		for (int i=0; i<stats.size(); i++) {
			proportions.push_back(0);
		}
		iterateProportionsStep(stats, proportions, 0, N, total, std::forward<Fn>(fn));
	}
	template <typename Fn>
	static void iterateProportionsStep(
		const std::vector<Stats>& stats,
		std::vector<int>& proportions,
		int id, int N, int total, Fn&& fn) {


		if (total == 0) {
			for (int i=id; i<stats.size(); i++) {
				proportions[i] = 0;
			}
			fn(stats, proportions);
			return;
		}
		if (id == N-1) {
			proportions[id] = total;
			fn(stats, proportions);
			return;
		}
		for (int i=0; i<=total; i++) {
			proportions[id] = i;
			iterateProportionsStep(stats, proportions, id+1, N, total-i, std::forward<Fn>(fn));
		}
	}
	static void run() {

		std::ifstream inputStream("2015/15.txt");
		std::regex pattern(R"((.+): capacity (-\d+|\d+), durability (-\d+|\d+), flavor (-\d+|\d+), texture (-\d+|\d+), calories (-\d+|\d+))");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_search(line, match, pattern);
			std::string name = match[1].str();
			Stats stats{
				std::stoi(match[2].str()),
				std::stoi(match[3].str()),
				std::stoi(match[4].str()),
				std::stoi(match[5].str()),
				std::stoi(match[6].str()),
			};
			ingredients.emplace_back(stats);
		}
		long long max = 0;
		//iterate score calculator for every proportion
		iterateProportions(ingredients, ingredients.size(), 100, [&max]( const std::vector<Stats>& stats, const std::vector<int>& proportions) {
			Stats res {0,0,0,0,0};
			//std::cout<< "Checking proportions: ";
			for (int i=0; i < proportions.size(); i++) {
				res = res + stats[i] * proportions[i];
				//std::cout << proportions[i] << " ";
			}
			//std::cout << std::endl;
			if (res.score() > max) {
				max = res.score();
				//std::cout << "New max: " << max << std::endl;
			}
		});

		std::cout << max << std::endl;

		inputStream.close();
	}
	static void runPart2() {
		long long max = 0;
		//same shit but with different function
		iterateProportions(ingredients, ingredients.size(), 100, [&max]( const std::vector<Stats>& stats, const std::vector<int>& proportions) {
			Stats res {0,0,0,0,0};
			//std::cout<< "Checking proportions: ";
			for (int i=0; i < proportions.size(); i++) {
				res = res + stats[i] * proportions[i];
				//std::cout << proportions[i] << " ";
			}
			//std::cout << std::endl;
			if (res.calories != 500)
				return;
			if (res.score() > max) {
				max = res.score();
				//std::cout << "New max: " << max << std::endl;
			}
		});
		std::cout << max << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/