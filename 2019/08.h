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

//reading image from stream
struct Task
{
	static constexpr int WIDTH = 25;
	static constexpr int HEIGHT = 6;

	static inline std::vector<Grid<WIDTH, HEIGHT, int>> image;
	static void run() {

		std::ifstream inputStream("2019/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		char bit;
		int ptrW=0, ptrH=0, ptrL=0;
		image.emplace_back();
		while (inputStream >> bit)
		{
			image[ptrL].set(ptrW, ptrH, bit-'0');
			ptrW++;
			if (ptrW == WIDTH) {
				ptrW = 0;
				ptrH++;
			}
			if (ptrH == HEIGHT) {
				ptrH = 0;
				ptrL++;
				image.emplace_back();
			}
		}
		inputStream.close();
		int min0 = INT_MAX;
		Grid<WIDTH, HEIGHT, int>* min0layer = nullptr;
		for (auto& layer : image) {
			int cnt = layer.count([](int num){return num==0;});
			if (cnt < min0) {
				min0 = cnt;
				min0layer = &layer;
			}
		}
		int cnt1 = min0layer->count([](int num){return num==1;});
		int cnt2 = min0layer->count([](int num){return num==2;});
		std::cout << cnt1 * cnt2 << std::endl;
	}
	static void runPart2() {
		Grid<WIDTH, HEIGHT, int> buffer;
		buffer.clear(2);
		for (int i=image.size()-1; i>=0; i--) {
			const auto& layer = image[i];
			for (int x = 0; x<layer.getWidth(); x++) {
				for (int y = 0; y<layer.getHeight(); y++) {
					int pixel = layer.get(x,y);
					if (pixel == 0 || pixel == 1) {
						buffer.set(x,y,pixel);
					}
				}
			}
		}
		buffer.print([](int val){
			if (val == 1) return '#';
			return '.';
		});
	}
};

//-------------- NOTES AREA ----------------
/*

*/