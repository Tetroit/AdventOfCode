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


// connecting ports in a bridge

struct Component {
	int in;
	int out;
	std::string toString() {
		return std::to_string(in) + "/" + std::to_string(out);
	}
};
struct Task
{
	//contains duplicated components like a/b b/a
	static inline std::vector<Component> components;
	//port to component id
	static inline std::unordered_map<int, std::vector<int>> graph;

	static int strongestPath(int inPort = 0) {
		std::vector<int> path {};
		int max = 0;
		int length = 0;
		for (int i=0; i<graph[inPort].size(); i++) {
			int val = checkPath(graph[inPort][i], path);
			if (val > max) max = val;
		}
		return max;
	}
	static int longestPath(int inPort = 0) {
		std::vector<int> path {};
		int max = 0;
		int length = 0;
		for (int i=0; i<graph[inPort].size(); i++) {
			int val = checkLongestPath(graph[inPort][i], path, length);
			if (val > max) max = val;
		}
		return max;
	}
	static int checkPath (const int inNodeID, std::vector<int> path) {
		int max = 0;
		const Component& inNode = components[inNodeID];
		int inPort = inNode.out;
		path.emplace_back(inNodeID/2);
		for (int i=0; i<graph[inPort].size(); i++) {
			auto match = std::find(path.begin(), path.end(), graph[inPort][i]/2);
			if (match != path.end()) {
				continue;
			}
			Component& next = components[inPort];
			int childLength;
			auto val = checkPath(graph[inPort][i], path);
			if (val > max) {
				max = val;
			}
		}
		return max + inNode.in + inNode.out;
	}
	static int checkLongestPath (const int inNodeID, std::vector<int> path, int& length) {
		int max = 0;
		int maxLength = 0;
		const Component& inNode = components[inNodeID];
		int inPort = inNode.out;
		path.emplace_back(inNodeID/2);
		for (int i=0; i<graph[inPort].size(); i++) {
			auto match = std::find(path.begin(), path.end(), graph[inPort][i]/2);
			if (match != path.end()) {
				continue;
			}
			Component& next = components[inPort];
			int childLength;
			auto val = checkLongestPath(graph[inPort][i], path, childLength);
			if (childLength > maxLength) {
				maxLength = childLength;
				max = val;
			}
			if (childLength == maxLength && val > max) {
				max = val;
			}
		}
		length = maxLength + 1;
		return max + inNode.in + inNode.out;
	}
	static void run() {

		std::ifstream inputStream("2017/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		int a, b;
		char shit;
		while (inputStream >> a >> shit >> b)
		{
			components.emplace_back(a, b);
			components.emplace_back(b, a);
		}
		inputStream.close();
		for (int i=0;i<components.size(); i++) {
			if (components[i].in == 0) {
				graph[0].emplace_back(i);
			}
		}
		for (int i=0; i<components.size(); i++) {
			int port = components[i].out;
			if (graph.contains(port))
				continue;
			for (int j=0; j<components.size(); j++) {
				if (i == j) continue;
				if (port == components[j].in) {
					graph[port].emplace_back(j);
				}
			}
		}
		std::cout << strongestPath(0) << std::endl;
	}
	static void runPart2() {
		std::cout << longestPath(0) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/