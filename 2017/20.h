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

//particle simulation

using numT = long long;
struct ivec3 {
	numT x, y, z;
	ivec3 operator+(const ivec3& other) const {
		return ivec3(x + other.x, y + other.y, z + other.z);
	}
	void operator+=(const ivec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}
	ivec3 operator-(const ivec3& other) const {
		return ivec3(x - other.x, y - other.y, z - other.z);
	}
	void operator-=(const ivec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	numT manh() const {
		return llabs(x) + llabs(y) + llabs(z);
	}
	bool operator==(const ivec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

struct Particle {
	int id;
	ivec3 p, v, a;
	numT distManh(ivec3& o) const {
		return (p - o).manh();
	}
	numT distManh(Particle& o) const {
		return (p - o.p).manh();
	}
	void simulate() {
		v += a;
		p += v;
	}
	bool collide(Particle& other) const {
		return p == other.p;
	}
};

struct Task
{
	static inline std::vector<Particle> particles;
	static std::optional<int> resolveCollision(Particle& p1, Particle& p2) {
		ivec3 relP = p1.p - p2.p;
		ivec3 relV = p1.v - p2.v;
		ivec3 relA = p1.a - p2.a;
		numT a = relA.x;
		numT b = relV.x;
		numT c = relP.x;
		if (a % 2 != 0) return std::nullopt;
		numT qeB = a/2 + b;
		numT det = qeB * qeB - 2 * a * c;
		for (int i=0; i<particles.size(); i++) {}
	}
	static void run() {

		std::ifstream inputStream("2017/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int id = 0;
		while (std::getline(inputStream, line))
		{
			Particle particle;
			particle.id = id;
			std::istringstream ss(line);
			ss.ignore(3);
			char shit;
			ss >> particle.p.x >> shit >> particle.p.y >> shit >> particle.p.z;
			ss.ignore(6);
			ss >> particle.v.x >> shit >> particle.v.y >> shit >> particle.v.z;
			ss.ignore(6);
			ss >> particle.a.x >> shit >> particle.a.y >> shit >> particle.a.z;
			particles.push_back(particle);
			id++;
		}
		inputStream.close();
		numT minA = std::numeric_limits<numT>::max();
		numT minV = std::numeric_limits<numT>::max();
		numT minP = std::numeric_limits<numT>::max();
		int minId = -1;
		for (auto & particle : particles) {
			if (particle.p.manh() < minA) {
			}
			if (particle.a.manh() < minA ||
				particle.a.manh() == minA && (particle.v.manh() < minV ||
				particle.v.manh() == minV && particles[minId].p.manh() < minP)) {

				minA = particle.a.manh();
				minV = particle.v.manh();
				minP = particle.p.manh();
				minId = particle.id;
			}
		}
		std::cout << minId << " " << minA << std::endl;
	}
	static void runPart2() {
		const int iterations = 1000;
		std::vector<int> toDestroy;
		for (int it = 0; it < iterations; it++) {
			for (int i=0; i<particles.size()-1; i++) {
				for (int j=i+1; j<particles.size(); j++) {
					if (particles[i].collide(particles[j])) {
						if (std::ranges::find(toDestroy, i) == toDestroy.end()) {
							toDestroy.push_back(i);
						}
						if (std::ranges::find(toDestroy, j) == toDestroy.end()) {
							toDestroy.push_back(j);
						}
					}
				}
			}
			for (int i=toDestroy.size() - 1; i>=0; i--) {
				particles.erase(particles.begin() + toDestroy[i]);
			}
			toDestroy.clear();

			for (auto& particle : particles) {
				particle.simulate();
				if (particle.p.manh() > 1000000000ll)
					std::cout << "Particle " << particle.id << " is drifting far: " << particle.p.manh() << std::endl;
			}
		}


		std::cout << particles.size() << std::endl;
		//796
		//770
	}
};

//-------------- NOTES AREA ----------------
/*

*/