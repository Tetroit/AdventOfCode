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

//TODO: put a small description of the task
struct Task
{
	enum Dir {
		CLOCKWISE = -4,
		COUNTER_CLOCKWISE = 4,
	};
	enum Turn {
		RIGHT = -1,
		LEFT = 1,
	};
	enum Facing {
		F_NAN,
		F_UP,
		F_DOWN,
		F_LEFT,
		F_RIGHT,
	};
	static std::string GetFacing(int x, int y) {
		if (x == 0) {
			if (y > 0) return "DOWN";
			if (y < 0) return "UP";
		}
		if (y == 0) {
			if (x > 0) return "RIGHT";
			if (x < 0) return "LEFT";
		}
		return "NOT A FACING";
	}
	static long long areaFromDiagonal(int x1, int y1, int x2, int y2) {
		return ((long long)(abs(x2 - x1)+1)) * (abs(y2 - y1) + 1);
	}
	static int sgn(int n) {
		if (n == 0) return 0;
		if (n > 0) return 1;
		return -1;
	}
	static Turn turnBy3Points(int x1, int y1, int x2, int y2, int x3, int y3) {
		int dirX1 = sgn(x1 - x2);
		int dirY1 = sgn(y1 - y2);
		int dirX2 = sgn(x3 - x2);
		int dirY2 = sgn(y3 - y2);
		return static_cast<Turn>(dirX1 * dirY2 - dirX2 * dirY1);
	}
	static bool makesCave(int x1, int y1, int x2, int y2, int x3, int y3) {
		int xMin = std::min(x1, x2);
		int xMax = std::max(x1, x2);
		int yMin = std::min(y1, y2);
		int yMax = std::max(y1, y2);
		if (y3 > yMin && y3 < yMax && x3 > xMin && x3 < xMax) {
			return true;
		}
		return false;
	}
	static bool rectVSlineX(int x1, int y1, int x2, int y2, int lineX1, int lineX2, int lineY) {
		int xMin = std::min(x1, x2);
		int xMax = std::max(x1, x2);
		int yMin = std::min(y1, y2);
		int yMax = std::max(y1, y2);
		int lineXMin = std::min(lineX1, lineX2);
		int lineXMax = std::max(lineX1, lineX2);

		if (lineY <= yMin || lineY >= yMax) return false;
		if (lineXMin >= xMax || lineXMax <= xMin) return false;
		return true;
	}
	static bool rectVSlineY(int x1, int y1, int x2, int y2, int lineY1, int lineY2, int lineX) {
		int xMin = std::min(x1, x2);
		int xMax = std::max(x1, x2);
		int yMin = std::min(y1, y2);
		int yMax = std::max(y1, y2);
		int lineYMin = std::min(lineY1, lineY2);
		int lineYMax = std::max(lineY1, lineY2);

		if (lineX <= xMin || lineX >= xMax) return false;
		if (lineYMin >= yMax || lineYMax <= yMin) return false;
		return true;
	}
	static void run() {

		std::ifstream inputStream("2025/input/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::pair<int,int>> map;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			int x,y;
			char extra;
			ss >> x >> extra >> y;
			map.emplace_back(x,y);
		}
		inputStream.close();
		long long max = 0;
		for (int i=0; i < map.size(); i++) {
			auto& poleA = map[i];
			for (int j=0; j < i; j++) {
				auto& poleB = map[j];
				long long area = areaFromDiagonal(poleA.first, poleA.second, poleB.first, poleB.second);
				if (area > max)
					max = area;
			}
		}
		std::cout << max << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2025/input/09.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;
		std::vector<std::pair<int,int>> map;
		std::vector<Turn> turns;
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> edges;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			int x,y;
			char extra;
			ss >> x >> extra >> y;
			map.emplace_back(x,y);
		}
		inputStream.close();

		Dir dir;
		auto poleAit = map.end() - 1;
		auto poleBit = map.begin();
		auto poleCit = map.begin() + 1;
		int turnSum = 0;
		for (int i=0; i < map.size(); i++) {
			edges.emplace_back(*poleAit,*poleBit);
			Turn turn = turnBy3Points(
				poleAit->first, poleAit->second,
				poleBit->first, poleBit->second,
				poleCit->first, poleCit->second);
			turnSum += turn;
			turns.emplace_back(turn);
			poleAit++;
			if (poleAit == map.end())
				poleAit = map.begin();
			poleBit++;
			if (poleBit == map.end())
				poleBit = map.begin();
			poleCit++;
			if (poleCit == map.end())
				poleCit = map.begin();
		}
		std::cout << turnSum << std::endl;
		dir = static_cast<Dir>(turnSum);

		long long max = 0;
		for (int i=0; i < map.size(); i++) {
			int prevAId = i - 1;
			if (prevAId < 0) prevAId = map.size() - 1;

			auto& poleA = map[i];
			auto& prevA = map[prevAId];

			bool isInner =
				turns[i] == LEFT && dir == COUNTER_CLOCKWISE ||
				turns[i] == RIGHT && dir == CLOCKWISE;
			int dirX1 = sgn(prevA.first - poleA.first);
			int dirY1 = sgn(prevA.second - poleA.second);
			int dirX2 = turns[i] == LEFT ? -dirY1 : dirY1;
			int dirY2 = turns[i] == LEFT ? dirX1 : -dirX1;
			int dirX = dirX1 == 0 ? dirX2 : dirX1;
			int dirY = dirY1 == 0 ? dirY2 : dirY1;

			for (int j=0; j < i; j++) {
				auto& poleB = map[j];
				long long area = areaFromDiagonal(poleA.first, poleA.second, poleB.first, poleB.second);
				if (area <= max)
					continue;

				int dirXb = sgn(poleB.first - poleA.first);
				int dirYb = sgn(poleB.second - poleA.second);
				if ((dirXb == dirX && dirYb == dirY) != isInner && dirXb != 0 && dirYb != 0) {
					// std::cout << "------ negative space ------" << '\n';
					// std::string cornerType = isInner ? "Inner" : "Outer";
					// std::cout << "cornerA: " << poleA.first << " " << poleA.second << '\n';
					// std::cout << "cornerB: " << poleB.first << " " << poleB.second << '\n';
					// std::cout << "corner type: " << cornerType << '\n';
					// std::cout << "area goes: " << GetFacing(dirX, 0) << " and " << GetFacing(0, dirY) << '\n';
					// std::cout << "rect goes: " << GetFacing(dirXb, 0) << " and " << GetFacing(0, dirYb) << '\n';
					continue;
				}
				bool isValid = true;
				for (auto& [edgeA, edgeB] : edges) {
					if (edgeA.first != edgeB.first && edgeA.second != edgeB.second) {
						std::cerr << "edge is not axis aligned" << '\n';
					}
					if (edgeA.first == edgeB.first && rectVSlineY(
						poleA.first, poleA.second,
						poleB.first, poleB.second,
						edgeA.second, edgeB.second,
						edgeA.first
					)) {
						isValid = false;
						break;
					}
					if (edgeA.second == edgeB.second && rectVSlineX(
						poleA.first, poleA.second,
						poleB.first, poleB.second,
						edgeA.first, edgeB.first,
						edgeA.second
					)) {
						isValid = false;
						break;
					}
				}
				if (isValid)
					max = area;
			}
		}
		std::cout << max << std::endl;
		//1221931480
	}
};

//-------------- NOTES AREA ----------------
/*

*/