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

//moving cucumbers

struct Task
{
	static inline DynamicGrid<char> init;
	static void run() {

		std::ifstream inputStream("2021/25.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}
		init.fillFromStream(inputStream, DefaultConvert<char>());

		auto cucumbers1 = init;
		auto cucumbers2 = init;
		auto* src = &cucumbers1;
		auto* dst = &cucumbers2;
		
		inputStream.close();
		bool changed = false;
		int iteration = 0;
		do {
			changed = false;
			iteration++;
			dst->clear('.');

			for (int x = 0; x<src->getWidth(); x++) {
				for (int y = 0; y<src->getHeight(); y++) {
					char symb = src->get(x,y);
					char next = src->get((x+1)%src->getWidth(),y);
					if (symb == 'v') dst->set(x,y,'v');
					if (symb != '>') continue;

					if (next == '.') {
						dst->set((x+1)%src->getWidth(),y,'>');
						changed = true;
					}
					else {
						dst->set(x,y,'>');
					}
				}
			}

			std::swap(src, dst);
			dst->clear('.');

			for (int x = 0; x<src->getWidth(); x++) {
				for (int y = 0; y<src->getHeight(); y++) {
					char symb = src->get(x,y);
					char next = src->get(x,(y+1)%src->getHeight());
					if (symb == '>') dst->set(x,y,'>');
					if (symb != 'v') continue;

					if (next == '.') {
						dst->set(x,(y+1)%src->getHeight(),'v');
						changed = true;
					}
					else {
						dst->set(x,y,'v');
					}
				}
			}

			std::swap(src, dst);
			// src->print(DefaultConvert<char>());
			std::cout << std::endl;

		} while (changed);
		
		src->print(DefaultConvert<char>());
		std::cout << iteration << std::endl;
	}
	static void runPart2() {

	}
};

//-------------- NOTES AREA ----------------
/*

*/