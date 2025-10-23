#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

struct Task
{

	struct Machine {
		int x1;
		int y1;
		int x2;
		int y2;
		int xRes;
		int yRes;

		//Cramer's rule
		int solve() const {

			int det = x1 * y2 - x2 * y1;
			int xDet = xRes * y2 - x2 * yRes;
			int yDet = x1 * yRes - xRes * y1;
			if (xDet % det != 0)
				return 0;
			if (yDet % det != 0)
				return 0;
			return xDet/det * 3 + yDet/det;
		}
		//part 2
		long long solve2() const {

			long long det = x1 * y2 - x2 * y1;
			long long xDet = (long long)(xRes * y2 - x2 * yRes) + 10000000000000 * (long long)(y2 - x2);
			long long yDet = (long long)(x1 * yRes - xRes * y1) + 10000000000000 * (long long)(x1 - y1);
			if (xDet % det != 0)
				return 0;
			if (yDet % det != 0)
				return 0;
			return xDet/det * 3 + yDet/det;
		}
	};

	void run() {


		std::ifstream inputStream("input2024-13.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		char ch;
		int num;
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0, xRes = 0, yRes = 0;
		int inputId = 0;

		std::vector<Machine> machines;
		while (inputStream >> ch)
		{
			if (ch == '+' || ch == '=') {
				inputStream >> num;
				switch (inputId) {
					case 0: x1 = num; break;
					case 1: y1 = num; break;
					case 2: x2 = num; break;
					case 3: y2 = num; break;
					case 4: xRes = num; break;
					case 5: yRes = num; break;
					default: std::cerr << "Wrong input!\n";
				}
				inputId++;
			}
			if (inputId == 6) {
				machines.push_back(Machine {
					x1, y1, x2, y2, xRes, yRes
				});
				inputId = 0;
			}
		}

		int cnt = 0;
		long long cnt2 = 0;
		for (const auto& machine : machines) {

			int res = machine.solve();
			long long res2 = machine.solve2();

			std::cout << res << ' ' << res2 << '\n';

			cnt += res;
			cnt2 += res2;
		}

		std::cout << cnt << ' ' << cnt2 << '\n';
	}
};
