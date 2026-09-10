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

//Figuring out which ingredients have allergens

struct Task
{
	static inline std::regex pattern = std::regex(R"((.+) \(contains (.+)\))");
	static inline std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> dishes;
	static inline std::unordered_map<std::string, std::vector<int>> allergies;
	static inline std::unordered_map<std::string, std::vector<int>> ingredients;
	static inline std::unordered_map<std::string, std::string> allergyToIngredient;
	static inline std::unordered_map<std::string, std::string> ingredientToAllergy;
	static void run() {

		std::ifstream inputStream("2020/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int row = 0;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			std::regex_match(line, match, pattern);
			auto ingredientList = Utils::split(match[1].str(), ' ');
			auto allergyList = Utils::split(match[2].str(), ", ");
			dishes.emplace_back(ingredientList, allergyList);
			for (auto& allergy : allergyList) {
				allergies[allergy].push_back(row);
			}
			for (auto& ingredient : ingredientList) {
				ingredients[ingredient].push_back(row);
			}
			row++;
		}
		inputStream.close();
		std::unordered_map<std::string, std::vector<std::string>> allergyCandidates;
		for (auto& [allergy, list] : allergies) {
			auto& candidates = allergyCandidates[allergy];
			candidates = dishes[list[0]].first;
			for (int i=1; i<list.size(); i++) {
				auto& compare = dishes[list[i]].first;
				for (int a=candidates.size() - 1; a>=0; a--) {
					if (std::find(compare.begin(), compare.end(), candidates[a]) == compare.end()) {
						candidates.erase(candidates.begin() + a);
					}
				}
			}
		}
		for (int loop = 0; loop < allergyCandidates.size(); loop++) {
			for (const auto& [key, candidates] : allergyCandidates) {
				if (allergyToIngredient.contains(key)) continue;
				if (candidates.size() == 1) {
					auto match = candidates[0];
					allergyToIngredient[key] = match;
					ingredientToAllergy[match] = key;
					for (auto& [key2, candidates2] : allergyCandidates) {
						if (&key2 == &key) continue;
						auto loc = std::find(candidates2.begin(), candidates2.end(), match);
						if (loc != candidates2.end()) {
							candidates2.erase(loc);
						}
					}
					break;
				}
			}
		}
		int cnt = 0;
		for (auto& dish : dishes) {
			for (auto& ingredient : dish.first) {
				if (!ingredientToAllergy.contains(ingredient)) {
					cnt++;
				}
			}
		}
		std::cout << cnt << std::endl;

	}
	static void runPart2() {
		std::vector<std::pair<std::string, std::string>> pairs;
		for (auto [al, in] : allergyToIngredient) {
			pairs.emplace_back(al, in);
		}
		std::sort(pairs.begin(), pairs.end());
		for (auto& [al, in] : pairs) {
			std::cout << in << ',';
		}
		std::cout << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/