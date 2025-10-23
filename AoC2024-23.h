#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{

	std::unordered_map<int, std::vector<int>> graph;
	std::vector<std::vector<int>> tris;

	std::vector<std::vector<int>> fullGraphs;

	void addConnection(int comp1, int comp2) {
		auto& arr = graph[comp1];
		for (int & i : arr) {
			if (areConnected(i, comp2)) {
				tris.push_back({i, comp1, comp2});
			}
		}
		graph[comp1].push_back(comp2);
		graph[comp2].push_back(comp1);

	}

	bool areConnected (int comp1, int comp2) const {
		for (auto& v : graph.at(comp1)) {
			if (v == comp2) {
				return true;
			}
		}
		return false;
	}



	bool searchBiggestFull(std::vector<std::vector<int>>& candidates, int id, int& maxSize, std::vector<int>& best) {
		std::vector<int> candidate = candidates.at(id);

		if (candidate.size() <= maxSize) {
			candidates.erase(candidates.begin() + id);
			return false;
		}

		bool isFull = true;
		int member = 0;

		std::vector<int> connected;

		for (int i=0; i < candidate.size(); i++) {

			member = candidate[i];
			for (int j = 0; j < candidate.size(); j++) {
				if (i == j)
					continue;
				if (!areConnected(member, candidate[j])) {
					isFull = false;
				}
				else {
					connected.push_back(candidate[j]);
				}
			}
			if (!isFull) {
				break;
			}
			connected.clear();
		}

		if (isFull) {
			maxSize = candidate.size();
			best = candidate;
			candidates.erase(candidates.begin() + id);
			std::cout << "New best: " << candidate.size() << '\n';
			return true;
		}

		candidates.erase(candidates.begin() + id);

		auto& lesser = candidates.emplace_back(connected);
		lesser.push_back(member);

		searchBiggestFull (candidates, candidates.size() - 1, maxSize, best);

		auto& bigger = candidates.emplace_back();
		for (int i : candidate) {
			if (i != member) {
				bigger.push_back(i);
			}
		}

		searchBiggestFull (candidates, candidates.size() - 1, maxSize, best);

		return false;
	}

	void run() {

		std::ifstream inputStream("input2024-23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::string computer1;
		std::string computer2;

		while (std::getline(inputStream, line))
		{
			int comp1 = (line[0] << 8) + line[1];
			int comp2 = (line[3] << 8) + line[4];
			addConnection(comp1, comp2);
		}

		int cnt = 0;
		int offset = 0;
		int size = 0;
		for (auto& arr : tris) {
			bool hasT = false;
			for (int i = 0; i < 3; i++) {
				if (arr[i] >> 8 == 't')
					hasT = true;
			}
			if (hasT) {
				cnt++;
			}
		}
		std::cout << cnt << "\n";

		std::vector<std::vector<int>> allVerts;
		allVerts.emplace_back();
		for (auto& [vert, arr] : graph) {
			allVerts[0].emplace_back(vert);
		}

		int maxSize = 3;
		std::vector<int> best;
		searchBiggestFull (allVerts, 0, maxSize, best);
		std::cout << maxSize << "\n";

		std::sort(best.begin(), best.end());
		for (auto& v : best) {
			std::cout << (char)(v >> 8) << (char)(v & 0xff) << ',';
		}

	}
};
