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

#include "vec.h"
#include "utils.h"


struct Bot {
	llvec3 pos;
	long long r;
};
struct Task
{
	static inline std::vector<Bot> bots;
	static void run() {

		std::ifstream inputStream("2018/23.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			Bot b;
			char _;
			std::istringstream iss(line);
			iss.ignore(sizeof("pos=<")-1);
			iss >> b.pos.x >> _ >> b.pos.y >> _ >> b.pos.z;
			iss.ignore(sizeof(">, r=")-1);
			iss >> b.r;
			bots.push_back(b);
		}
		inputStream.close();
		int max = std::numeric_limits<int>::min();
		int maxId = -1;
		for (int i=0; i<bots.size(); i++) {
			if (max < bots[i].r) {
				max = bots[i].r;
				maxId = i;
			}
		}
		int cnt = 0;
		for (int i=0; i<bots.size(); i++) {
			if ((bots[i].pos - bots[maxId].pos).len() <= max) cnt++;
		}
		std::cout << cnt << std::endl;
	}
	static std::vector<std::vector<int>> findMaximalClique(const std::unordered_map<int, std::vector<int>>& graph, int i, int& maxSize) {
		std::vector<int> subgraph {i};
		for (auto& connection : subgraph) {

		}
	}
	static void runPart2() {
		long long minDist = std::numeric_limits<long long>::max();
		std::unordered_map<int, std::vector<int>> graph;
		for (int i=0; i<bots.size()-1; i++) {
			for (int j=i+1; j<bots.size(); j++) {
				long long len = (bots[i].pos - bots[j].pos).len();
				if (len <= (bots[j].r + bots[i].r)) {
					graph[i].emplace_back(j);
					graph[j].emplace_back(i);
					long long overlap = bots[j].r + bots[i].r - len;
					if (overlap < minDist) {
						minDist = overlap;
					}
				}
			}
		}
		std::vector<int> largestSub;
		for (int i=0; i<bots.size(); i++) {
			if (graph[i].size() < 980) {
				for (auto& connection : graph[i]) {
					graph[connection].erase(std::find(graph[connection].begin(), graph[connection].end(), i));
				}
				graph[i].clear();
			}
		}
		for (int i=0; i<bots.size(); i++) {
			if (graph[i].size() >= 980) {
				largestSub.push_back(i);
			}
		}
		// for (int i=largestSub.size()-1; i>=0; i--) {
		// 	for (int j=0; j<largestSub.size(); j++) {
		// 		auto& connections = graph[largestSub[j]];
		// 		if (std::find(connections.begin(), connections.end(), i) == connections.end()) {
		// 			largestSub.erase(largestSub.begin()+j);
		// 		}
		// 		graph[largestSub[i]].emplace_back(largestSub[j]);
		// 	}
		// }
		// long long minX = LLONG_MAX, minY = LLONG_MAX, minZ = LLONG_MAX;
		// long long maxX = LLONG_MIN, maxY = LLONG_MIN, maxZ = LLONG_MIN;
		// bool first = true;
		// for (auto& el : largestSub) {
		// 	auto& bot = bots[el];
		// 	if (first) {
		// 		minX = bot.pos.x - bot.r;
		// 		minY = bot.pos.y - bot.r;
		// 		minZ = bot.pos.z - bot.r;
		// 		maxX = bot.pos.x + bot.r;
		// 		maxY = bot.pos.y + bot.r;
		// 		maxZ = bot.pos.z + bot.r;
		// 		first = false;
		// 	}
		// 	minX = std::max(minX, bot.pos.x - bot.r);
		// 	minY = std::max(minY, bot.pos.y - bot.r);
		// 	minZ = std::max(minZ, bot.pos.z - bot.r);
		// 	maxX = std::min(maxX, bot.pos.x + bot.r);
		// 	maxY = std::min(maxY, bot.pos.y + bot.r);
		// 	maxZ = std::min(maxZ, bot.pos.z + bot.r);
		// }
		// std::cout << minX << " " << minY << " " << minZ << std::endl;
		// std::cout << maxX << " " << maxY << " " << maxZ << std::endl;
		// std::cout << abs(minX) + abs(minY) + abs(minZ) << std::endl;
		// std::cout << abs(maxX) + abs(maxY) + abs(maxZ) << std::endl;

		long long maxDist = LLONG_MIN;
		for (auto& el : largestSub) {
			const auto& bot = bots[el];
			auto distTillCenter = bot.pos.len() - bot.r;
			if (distTillCenter > maxDist) {
				maxDist = distTillCenter;
			}
		}
		maxDist++;
		std::cout << maxDist << std::endl;
		// int maxCnt = 0;
		//LAZY MANUAL BRUTEFORCE "GRADIENT DESCEND" that confirms I was 1 unit off (dont do that)
		// for (int x = 54127930 - 100; x<54128010 + 1000; x+=1) {
		// 	for (int z = 30447931 - 100; z < 30447931 + 1000; z+=1) {
		// 		cnt = 0;
		// 		long long y = maxDist - x - z + 1;
		// 		testBot.pos = {x, y, z};
		// 		for (auto& el : largestSub) {
		// 			auto& bot = bots[el];
		// 			long long len = (bot.pos - testBot.pos).len();
		// 			if (len <= (bot.r)) {
		// 				cnt++;
		// 			}
		// 		}
		// 		if (maxCnt < cnt) {
		// 			maxCnt = cnt;
		// 			std::cout << maxCnt << std::endl;
		// 			std::cout << x << " " << y << " " << z << std::endl;
		// 			std::cout << x + y + z << std::endl;
		// 		}
		// 	}
		// }
		//101599539 low
		//82032490 low
		// for (auto& [id, connections] : graph) {
		// 	std::cout << connections.size() << std::endl;
		// }
		// std::cout << minDist << std::endl;
		// llvec3 LOPos1 = bots[lowestOverlap1].pos;
		// llvec3 LOPos2 = bots[lowestOverlap2].pos;
		// std::cout << LOPos1.x << ',' << LOPos1.y << ',' << LOPos1.z  << std::endl;
		// std::cout << LOPos2.x << ',' << LOPos2.y << ',' << LOPos2.z  << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

aint no way im doing maximum clique algo on 1000 vertices :skull:
judging by connection graph there seems to be a very large overlap between 980/1000 bots
if this is true then I could try masking vertices with power less than 980

that is exactly the case

*/