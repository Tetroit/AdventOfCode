#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <numeric>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"
#include "vec.h"

//PHYSICS!! YIPPEE!

struct Body {
	ivec3 pos = {0,0,0};
	ivec3 vel = {0,0,0};
};

struct Task
{
	static inline std::vector<Body> initial;

	static int sgn (int a, int b) {
		if (b > a)	return 1;
		if (b < a)	return -1;
		return 0;
	}
	static ivec3 sgn (ivec3 posA, ivec3 posB) {
		ivec3 res;
		for (int i=0; i<3; i++) {
			res.data[i] = sgn(posA.data[i], posB.data[i]);
		}
		return res;
	}
	static void simulate(std::vector<Body>& collection) {
		for (int i = 0; i < collection.size()-1; i++) {
			Body& a = collection[i];
			for (int j=i+1; j<collection.size(); j++) {
				Body& b = collection[j];
				auto gravA = sgn(a.pos, b.pos);
				auto gravB = -gravA;
				a.vel+=gravA;
				b.vel+=gravB;
			}
		}
		for (auto& body : collection) {
			body.pos += body.vel;
		}
	}
	static void run() {

		std::ifstream inputStream("2019/12.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			Body body;
			std::istringstream ss(line);
			ss.ignore(3);
			ss >> body.pos.x;
			ss.ignore(4);
			ss >> body.pos.y;
			ss.ignore(4);
			ss >> body.pos.z;
			initial.push_back(body);
		}
		inputStream.close();

		std::vector<Body> bodies = initial;
		for (int i=0; i<1000; i++) {
			simulate(bodies);
		}

		int totalEnergy = 0;
		for (auto& body : bodies) {
			totalEnergy += body.pos.len() * body.vel.len();
		}
		std::cout << totalEnergy << std::endl;
	}
	static void runPart2() {

		auto bodies = initial;

		llvec3 period = {-1,-1,-1};
		long long foundPeriods = 0;
		long long iteration = 0;
		while (foundPeriods != 3) {
			simulate(bodies);
			iteration++;
			for (int i=0; i<3; i++) {
				if (period.data[i] != -1) continue;
				bool isSame = true;
				for (int b=0; b<bodies.size(); b++) {
					if (bodies[b].pos.data[i] != initial[b].pos.data[i] ||
						bodies[b].vel.data[i] != initial[b].vel.data[i]) {
						isSame = false;
						break;
					}
				}
				if (isSame) {
					foundPeriods++;
					period.data[i] = iteration;
				}
			}
		}

		std::cout << std::lcm(std::lcm(period.x, period.y), period.z) << std::endl;

	}
};

//-------------- NOTES AREA ----------------
/*

	consider there exist 2 distinct states which will result in the same state after 1 simulation step
	then for each dimension X
	x1 = x0 + vx0 + ax0 = x0' + vx0' + ax0'
	vx1 = vx0 + ax0 = vx0' + ax0'
	where x0 != x0' or vx0 != vx0' or ax0 != ax0'
	since vx0 + ax0 = vx0' + ax0' then x0 = x0'
	since x0 = x0' then ax0 = ax0' due to acceleration being determined only from positions, which we've proven to be invariant
	this means vx0 = vx0' and thus our initial statement is wrong and there is only 1 previous state for each state
	therefore the simulation is deterministic meaning the initial state will always be the one repeated first

	since the 3 dimensions can work separately there will be cases when 1 axis aligns with initial state and 3 others does not
	which happens periodically
	so there will be 3 periods along 3 axes and least common multiple of them should be the answer

*/