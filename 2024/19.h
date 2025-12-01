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

	bool canBeNext(const std::string& towel, const std::string& pattern, int id) {

		if (towel.size() - id < pattern.size())
			return false;
		return (towel.substr(id,pattern.length()) == pattern);
	}
	bool check(const std::string& towel, const std::vector<std::string>& patterns, int id = 0, const std::string& exception = "") {

		bool result = false;
		for (int i = 0; i < patterns.size(); ++i) {

			auto& pattern = patterns.at(i);
			if (pattern == exception)
				continue;

			if (canBeNext(towel, pattern, id)) {
				if (id + pattern.length() == towel.length()) {
					return true;
				}

				if (check(towel, patterns, id + pattern.length(), exception)) {
					return true;
				}
			}
		}
		return false;
	}

	int maxLength(const std::vector<std::string>& patterns) {
		int result = 0;
		for (auto& pattern : patterns) {
			if (pattern.length() > result) {
				result = pattern.length();
			}
		}
		return result;
	}

	long long countOptions(const std::string& towel, const std::vector<std::string>& patterns) {

		std::vector<long long> dp(towel.size() + 1, 0ll);
		dp[0] = 1;

		for (int i=0; i<towel.size(); ++i) {
			for (const auto &pattern : patterns) {
				if (canBeNext(towel, pattern, i)) {
					dp[i + pattern.length()] += dp[i];
				}
			}
		}
		return dp.back();
	}

	void run() {

		std::ifstream inputStream("input2024-19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::getline (inputStream, line);

		auto patterns = split(line, ", ");
		std::vector<std::string> towels;
		std::getline (inputStream, line);

		while (std::getline (inputStream, line))
		{
			towels.push_back(line);
		}

		std::vector<std::string> toErase;
		std::vector<int> toEraseIDs;

		// consider elementary fragments sequence:
		// 1|2|3|4|5|6|7|8|9
		// s - 1 cuts
		// (s-1)^2 possible cuts (64 in this case)
		//
		// 12 does not exist?
		// 12|3?4?5?6?7?8?9 doesn't work -> - 1/4 of all options
		// for edge cases, LACK of segment results in - 1/(2^l) of options
		// for middle cases - 1/(2^(l+1)) of options
		//
		// total = w ^ (s-1)
		// 12..n exists?
		// YES
		// recurse (n+1,n+2...)
		// NO
		// eliminate 2^(s-l-1)
		// return total
		//
		// exceptions:
		// if s = 1 return 1
		// worst case - 2^N
		// use word break DP

		// for (int i = 0; i < patterns.size(); i++) {
		// 	auto pattern = patterns.at(i);
		// 	if (pattern.length() == 1) {
		// 		continue;
		// 	}
		// 	if (check(pattern, patterns, 0, pattern))
		// 	{
		// 		toErase.push_back(pattern);
		// 		toEraseIDs.push_back(i);
		// 	}
		// }
		//
		// for (int i = toEraseIDs.size() -1 ; i >= 0; i--) {
		// 	patterns.erase(patterns.begin() + toEraseIDs.at(i));
		// }

		long long cnt = 0;
		for (auto& towel : towels) {
			long long res = countOptions(towel, patterns);
			cnt += res;
			std::cout << res << '\n';
		}
		std::cout << cnt << '\n';
	}
};
