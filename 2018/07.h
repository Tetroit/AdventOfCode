#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//Running programs in correct order
struct Task
{
	static inline std::unordered_map<char, std::vector<char>> graph;
	static inline std::unordered_map<char, std::vector<char>> unlockGraph;
	static inline std::unordered_set<char> steps;
	static inline std::unordered_set<char> processed;
	static inline std::deque<char> order;

	static void UnlockSteps(char process) {
		for (auto dep : unlockGraph[process]) {
			bool canUnlock = true;
			for (auto req : graph[dep]) {
				if (processed.count(req) != 1) {
					canUnlock = false;
					break;
				}
			}
			if (canUnlock) {
				bool placed = false;
				for (int i=0; i<order.size(); i++) {
					if (order[i] < dep) {
						order.insert(order.begin()+i, dep);
						placed = true;
						break;
					}
				}
				if (!placed) {
					order.push_back(dep);
				}
			}
		}
	}
	static int GetTime(char process) {
		return (int)(process - 'A') + 61;
	}
	static int NextWorker(const std::vector<int>& workers, const std::vector<uint8_t>& waiting) {
		int min = std::numeric_limits<int>::max();
		int minID = -1;
		for (int i=0; i<workers.size(); i++) {
			if (waiting[i]) continue;
			if (workers[i] < min) {
				min = workers[i];
				minID = i;
			}
		}
		return minID;
	}

	static void run() {

		std::ifstream inputStream("2018/07.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string shit;
		char req, target;
		while (inputStream >> shit >> req >> shit >> shit >> shit >> shit >> shit >> target >> shit >> shit)
		{
			graph[target].push_back(req);
			unlockGraph[req].push_back(target);
			steps.insert(req);
			steps.insert(target);
		}
		for (char step = 'A' ; step < 'A' + (char)steps.size() ; step++) {
			if (!graph.contains(step))
				order.emplace_back(step);
		}
		inputStream.close();
		std::string res;
		for (int i=0; i<steps.size(); i++) {
			for (char step = 'A' ; step < 'A' + (char)steps.size() ; step++) {
				if (processed.count(step) == 1) continue;
				bool canPrint = true;
				for (auto& dep : graph[step]) {
					if (processed.count(dep) != 1) {
						canPrint = false;
						break;
					}
				}
				if (canPrint) {
					res+=step;
					processed.emplace(step);
					break;
				}
			}
			if (processed.size() == steps.size()) {
				break;
			}
		}
		std::cout << res << std::endl;
	}

	static void Wait(std::vector<int>& clock, std::vector<std::optional<char>>& workers, int waiter) {
		int minWait = std::numeric_limits<int>::max();
		for (int i=0; i<workers.size(); i++) {
			if (i == waiter) continue;
			if (workers[i].has_value()) {
				if (clock[i] < minWait) {
					minWait = clock[i];
				}
			}
		}
		workers[waiter] = std::nullopt;
		clock[waiter] = minWait;
	}
	static void runPart2() {
		std::string res;
		processed.clear();
		std::vector<int> workerClock(5, 0);
		std::vector<uint8_t> waiting(5, 0);
		std::vector<std::optional<char>> worker(5, std::nullopt);
		while (true) {
			int id = NextWorker(workerClock, waiting);
			if (worker[id].has_value()) {
				char val = worker[id].value();
				processed.emplace(val);
				res+=val;
				if (processed.size() == steps.size()) break;
				UnlockSteps(val);
				for (auto& flag : waiting) flag = false;
			}
			if (order.empty()) {
				Wait(workerClock, worker, id);
				waiting[id] = true;
				continue;
			}
			char task = order.front();
			order.pop_front();
			workerClock[id] += GetTime(task);
			worker[id] = task;
		}
		int max = workerClock[0];
		for (int i : workerClock) {
			if (max < i) {
				max = i;
			}
		}
		std::cout << max << std::endl;
		std::cout << res << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/