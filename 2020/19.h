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

//lines matching recursive rules

struct Rule {
	std::optional<char> ch;
	std::vector<std::vector<int>> patterns;
	std::unordered_set<int> possibleLengths;

	void precalculateLengthRange(std::unordered_map<int, Rule>& rules, int cap, int depth) {
		if (depth >= cap) {
			return;
		}
		if (ch.has_value()) {
			possibleLengths = {1};
			return;
		}
		for (auto& variant : patterns) {
			std::unordered_set<int> variantLengths{0};
			for (int rule : variant) {
				rules[rule].precalculateLengthRange(rules, cap, depth + 1);
				variantLengths = Utils::minkovskySum(variantLengths, rules[rule].possibleLengths);
			}
			for (auto& len : variantLengths) {
				possibleLengths.insert(len);
			}
		}
	}
	bool match(const std::string& token, std::unordered_map<int, Rule>& rules) {

		int tokenSize = token.size();
		if (!possibleLengths.contains(token.size())) {
			return false;
		}
		if (tokenSize == 1 && ch.has_value()) {
			return token[0] == ch.value();
		}
		for (auto & variant : patterns) {
			for (auto lens : getSplits(rules, variant, tokenSize)) {
				bool valid = true;
				int offset = 0;
				for (int i=0; i<lens.size(); ++i) {
					auto part = token.substr(offset, lens[i]);
					offset+=lens[i];
					if (! rules[variant[i]].match(part, rules)) {
						valid = false;
						break;
					}
				}
				if (valid) return true;
			}
		}
		return false;
	}


	std::vector<std::vector<int>> getSplits(std::unordered_map<int, Rule>& rules, std::vector<int>& list, int size) {
		std::vector<std::vector<int>> res;
		getSplitsIt(rules, res, size, 0, list, {});
		return res;
	}
	void getSplitsIt(std::unordered_map<int, Rule>& rules, std::vector<std::vector<int>>& splits, int remaining, int n, std::vector<int>& list, std::vector<int> stack) {
		auto& rule =  rules[list[n]];
		if (n == list.size()-1) {
			if (rule.possibleLengths.contains(remaining)) {
				auto newSplit = stack;
				newSplit.push_back(remaining);
				splits.push_back(newSplit);
			}
			return;
		}
		for (auto len : rule.possibleLengths) {
			stack.push_back(len);
			getSplitsIt(rules, splits, remaining-len, n+1, list, stack);
			stack.pop_back();
		}
	}
};

struct Task
{
	static inline std::unordered_map<int, Rule> rules;
	static inline std::vector<std::string> patterns;
	static void run() {

		std::ifstream inputStream("2020/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		bool inputRules = true;
		while (std::getline(inputStream, line))
		{
			if (line == "") {
				inputRules = false;
				continue;
			}
			if (inputRules) {
				int keySep = line.find_first_of(':');
				int key = std::stoi(line.substr(0, keySep));
				line = line.substr(keySep + 2);
				auto rest = Utils::split(line, ' ');

				Rule& newRule = rules[key];
				if (rest[0][0] == '\"' ) {
					newRule.ch = rest[0][1];
					continue;
				}
				newRule.patterns.emplace_back();
				for (std::string& word : rest) {
					if (word == "|") {
						newRule.patterns.emplace_back();
						continue;
					}
					newRule.patterns.back().push_back(std::stoi(word));
				}
			}
			else {
				patterns.push_back(line);
			}
		}
		inputStream.close();

		rules[0].precalculateLengthRange(rules, 40, 0);
		int cnt = 0;
		for (auto& pattern : patterns) {
			if (rules[0].match(pattern, rules)) cnt++;
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		rules[8].patterns.emplace_back(std::vector<int>{42, 8});
		rules[11].patterns.emplace_back(std::vector<int>{42, 11, 31});

		for (auto& [id, rule] : rules) {
			rule.possibleLengths.clear();
		}
		rules[0].precalculateLengthRange(rules, 40, 0);
		int cnt = 0;
		for (auto& pattern : patterns) {
			if (rules[0].match(pattern, rules)) cnt++;
		}
		std::cout << cnt << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	the ultimate method that can deal with any type of grammar and loops! (tho it is slow and has to be capped manually haha)
*/