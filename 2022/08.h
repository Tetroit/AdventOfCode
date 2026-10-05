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

#include "GridBase.h"
#include "utils.h"

//counting visible trees

struct Task
{
	static inline DynamicGrid<int> trees;

	static bool isVisible(int x, int y, int h) {
		bool isVisible = true;
		for (int i=x-1; i>=0; i--) {
			if (trees.get(i,y) >= h) {
				isVisible = false;
				break;
			}
		}
		if (isVisible) return true;
		isVisible = true;
		for (int i=y-1; i>=0; i--) {
			if (trees.get(x,i) >= h) {
				isVisible = false;
				break;
			}
		}
		if (isVisible) return true;
		isVisible = true;
		for (int i=x+1; i<trees.getWidth(); i++) {
			if (trees.get(i,y) >= h) {
				isVisible = false;
				break;
			}
		}
		if (isVisible) return true;
		isVisible = true;
		for (int i=y+1; i<trees.getHeight(); i++) {
			if (trees.get(x,i) >= h) {
				isVisible = false;
				break;
			}
		}
		return isVisible;
	}

	static int viewScore(int x, int y, int h) {
		int total = 1;
		int score = 0;
		for (int i=x-1; i>=0; i--) {
			score++;
			if (trees.get(i,y) >= h) {
				break;
			}
		}
		total *= score;
		score = 0;
		for (int i=y-1; i>=0; i--) {
			score++;
			if (trees.get(x,i) >= h) {
				break;
			}
		}
		total *= score;
		score = 0;
		for (int i=x+1; i<trees.getWidth(); i++) {
			score++;
			if (trees.get(i,y) >= h) {
				break;
			}
		}
		total *= score;
		score = 0;
		for (int i=y+1; i<trees.getHeight(); i++) {
			score++;
			if (trees.get(x,i) >= h) {
				break;
			}
		}
		total *= score;
		return total;
	}
	static void run() {

		std::ifstream inputStream("2022/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		trees.fillFromStream(inputStream, [](char ch){return ch - '0';});
		inputStream.close();

		int cnt = 0;
		for (int x = 0; x < trees.getWidth(); x++) {
			for (int y = 0; y < trees.getHeight(); y++) {
				if (isVisible(x, y, trees.get(x,y))) cnt++;
			}
		}
		std::cout << cnt << std::endl;
	}
	static void runPart2() {
		int max = 0;
		for (int x = 0; x < trees.getWidth(); x++) {
			for (int y = 0; y < trees.getHeight(); y++) {
				int score = viewScore(x, y, trees.get(x,y));
				if (score > max) max = score;
			}
		}
		std::cout << max << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/