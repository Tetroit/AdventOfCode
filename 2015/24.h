#pragma once

#include <assert.h>
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

//putting presents in 3 groups with equal weights and min quantum entanglement (product of weights)
struct Task
{
	using GroupQE = long long;
	using GroupSet = std::vector<GroupQE>;

	static inline std::vector<int> weights;
	static inline int weightSum;

	static bool CanChooseSum(std::vector<int> arr, int sum) {
		std::vector<uint8_t> dp(sum+1, false);
		dp[0] = true;
		for (int & weight : arr) {
			for (int j = dp.size() - 1; j >= 0; --j) {
				if (dp[j]) {
					if (j+weight < dp.size()) {
						dp[j+weight] = true;
					}
				}
			}
		}
		std::cout << "Check failed";
		return dp.back();
	}
	template<typename T>
	static std::vector<T> Exclude(const std::vector<T>& main, const std::vector<T>& exclude) {
		std::vector<T> result;
		result.reserve(main.size());
		for (const T& a : main) {
			if (std::find(exclude.begin(), exclude.end(), a) == exclude.end()) {
				result.push_back(a);
			}
		}
		return result;
	}
	static GroupQE BestGroup() {
		int target = weightSum/3;
		//if we get more than 1/3 of elements in 1 group it is quaranteed that
		//in at least 1 of the other 2 groups will be less elements
		int maxWeights = weights.size()/3;
		GroupQE minQE = std::numeric_limits<GroupQE>::max();
		std::vector<std::vector<GroupQE>> dp_qe(target+1);
		std::vector<std::vector<std::vector<int>>> dp_group(target+1);
		dp_qe[0].push_back(1);
		dp_group[0].emplace_back();
		for (int w = weights.size() - 1; w >= 0; --w) {
			auto& weight = weights[w];
			for (int i=target - weight; i>=0; --i) {
				for (int combo = 0; combo < dp_qe[i].size(); combo++) {
					GroupQE qe = dp_qe[i][combo];
					std::vector<int> els = dp_group[i][combo];
					qe *= weight;
					els.push_back(weight);
					if (els.size() > maxWeights)
						continue;
					if (els.size() == maxWeights && qe > minQE) {
						continue;
					}
					if (i + weight == target /*&& CanChooseSum(Exclude(weights, els), target)*/) {
						maxWeights = els.size();
						minQE = qe;
						// std::cout << "New best: " << minQE << std::endl;
					}
					dp_qe[i+weight].push_back(qe);
					dp_group[i+weight].push_back(els);
				}
			}
		}

		return minQE;
	}
	static GroupQE BestGroup4() {
		int target = weightSum/4;
		//if we get more than 1/4 of elements in 1 group it is quaranteed that
		//in at least 1 of the other 3 groups will be less elements
		int maxWeights = weights.size()/4;
		GroupQE minQE = std::numeric_limits<GroupQE>::max();
		std::vector<std::vector<GroupQE>> dp_qe(target+1);
		std::vector<std::vector<std::vector<int>>> dp_group(target+1);
		dp_qe[0].push_back(1);
		dp_group[0].emplace_back();
		for (int w = weights.size() - 1; w >= 0; --w) {
			auto& weight = weights[w];
			for (int i=target - weight; i>=0; --i) {
				for (int combo = 0; combo < dp_qe[i].size(); combo++) {
					GroupQE qe = dp_qe[i][combo];
					std::vector<int> els = dp_group[i][combo];
					qe *= weight;
					els.push_back(weight);
					if (els.size() > maxWeights)
						continue;
					if (els.size() == maxWeights && qe > minQE) {
						continue;
					}
					if (i + weight == target) {
						maxWeights = els.size();
						minQE = qe;
						// std::cout << "New best: " << minQE << std::endl;
					}
					dp_qe[i+weight].push_back(qe);
					dp_group[i+weight].push_back(els);
				}
			}
		}

		return minQE;
	}
	static void run() {

		std::ifstream inputStream("2015/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int weight;
		while (inputStream >> weight)
		{
			weightSum += weight;
			weights.push_back(weight);
		}
		inputStream.close();
		GroupQE combo = BestGroup();
		std::cout << combo << std::endl;
	}
	static void runPart2() {
		GroupQE combo = BestGroup4();
		std::cout << combo << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*
	All input weights are prime, so we can identify each combination by quantum entanglement
*/