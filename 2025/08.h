#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//A task with connecting elements in circuits in a 3d space

template <typename T>
struct vec3 {
	T x, y, z;
	vec3 operator +(const vec3& other) const {
		return {x + other.x, y + other.y, z + other.z};
	}
	vec3 operator -(const vec3& other) const {
		return {x - other.x, y - other.y, z - other.z};
	}
	[[nodiscard]] T l_sq() const {
		return x * x + y * y + z * z;
	}
	static T dist_sq(const vec3& a, const vec3& b) {
		return (a - b).l_sq();
	}
};
struct Task
{
	typedef std::pair<long long, std::pair<int, int>> distancePair;
	typedef std::vector<distancePair> DistGraph;

	static void connectNClosest(const std::vector<vec3<long long>>& points, const DistGraph& distGraph,
		std::vector<std::vector<int>>& circuits, std::vector<int>& idToCircuit, int N) {
		for (int closestDist = 0; closestDist < N; closestDist++) {
			auto& [dist, pair] = distGraph.at(closestDist);
			int first = pair.first;
			int second = pair.second;
			int firstCircuit = idToCircuit[first];
			int secondCircuit = idToCircuit[second];
			if (idToCircuit.at(first) == idToCircuit.at(second)) {
				continue;
			}
			for (int j = circuits.at(secondCircuit).size() - 1; j>=0; j--) {
				int element = circuits[secondCircuit][j];
				circuits[secondCircuit].pop_back();
				circuits[firstCircuit].push_back(element);
				idToCircuit[element] = idToCircuit[first];
			}
		}
	}
	static void connectAll(const std::vector<vec3<long long>>& points, const DistGraph& distGraph,
		std::vector<std::vector<int>>& circuits, std::vector<int>& idToCircuit, int& last1, int& last2) {
		//if there are n - 1 connections in the graph, it is interconnected
		int connections = 0;

		//no need to sort or search as we have all closest points sorted in ascending order
		for (int closestDist = 0; connections < points.size() - 1; closestDist++) {
			auto& [dist, pair] = distGraph.at(closestDist);
			int first = pair.first;
			int second = pair.second;
			int firstCircuit = idToCircuit[first];
			int secondCircuit = idToCircuit[second];

			//nothing happens if in the same region
			if (idToCircuit.at(first) == idToCircuit.at(second)) {
				continue;
			}

			//otherwise connection successful -> move all elements from that region to the connected one
			for (int j = circuits.at(secondCircuit).size() - 1; j>=0; j--) {
				int element = circuits[secondCircuit][j];
				circuits[secondCircuit].pop_back();
				circuits[firstCircuit].push_back(element);
				idToCircuit[element] = idToCircuit[first];
			}
			last1 = first;
			last2 = second;
			connections++;
		}
	}

	static int biggest3CircuitsScore(const std::vector<std::vector<int>>& circuits) {
		int max1 = 0;
		int max2 = 0;
		int max3 = 0;
		for (const auto & circuit : circuits) {
			if (max1 < circuit.size()) {
				max3 = max2;
				max2 = max1;
				max1 = circuit.size();
			}
			else if (max2 < circuit.size()) {
				max3 = max2;
				max2 = circuit.size();
			}
			else if (max3 < circuit.size()) {
				max3 = circuit.size();
			}
		}
		return max3 * max2 * max1;
	}
	static void run() {

		std::ifstream inputStream("2025/input/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;

		std::vector<vec3<long long>> points;
		DistGraph distances;
		std::vector<std::vector<int>> regions;
		std::vector<int> idToRegions;

		int iLine =0;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			int x, y, z;
			char extra;
			ss >> x >> extra >> y >> extra >> z;
			points.push_back({x, y, z});
			regions.emplace_back();
			regions.back().push_back(iLine);
			idToRegions.emplace_back(iLine);
			iLine++;
		}
		inputStream.close();

		for (int i = 0; i < points.size(); i++) {

			for (int j = 0; j < i; j++) {
				if (i == j) continue;
				auto& vecI = points.at(i);
				auto& vecJ = points.at(j);
				long long dist = vec3<long long>::dist_sq(vecI, vecJ);
				distances.emplace_back(dist, std::make_pair(i, j));
			}
		}
		std::sort(distances.begin(), distances.end(), [](const distancePair& a, const distancePair& b) {
			return a.first < b.first;
		});
		connectNClosest(points, distances, regions, idToRegions, 1000);
		int score = biggest3CircuitsScore(regions);
		std::cout << score << std::endl;
	}
	static void runPart2() {
		std::ifstream inputStream("2025/input/08.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}

		std::string line;

		//keep all vertex data here
		//all vertices can be referred by their IDs
		std::vector<vec3<long long>> points;
		//store distance data as [distance, [first vertex ID, second vertex ID]]
		DistGraph distances;
		//store connection groups as [region ID, vertex ID]
		std::vector<std::vector<int>> regions;
		//have a reverse list to regions for faster lookup as [vertex ID, region ID]
		std::vector<int> idToRegions;

		int iLine =0;
		while (std::getline(inputStream, line))
		{
			std::stringstream ss(line);
			int x, y, z;
			char extra;
			ss >> x >> extra >> y >> extra >> z;
			points.push_back({x, y, z});

			//initially no 2 vertices are connected, so region IDs will match with vertex ID
			regions.emplace_back();
			regions.back().push_back(iLine);
			idToRegions.emplace_back(iLine);
			iLine++;
		}
		inputStream.close();

		//calculate distance links
		for (int i = 0; i < points.size(); i++) {
			for (int j = 0; j < i; j++) {
				if (i == j) continue;
				auto& vecI = points.at(i);
				auto& vecJ = points.at(j);
				long long dist = vec3<long long>::dist_sq(vecI, vecJ);
				distances.emplace_back(dist, std::make_pair(i, j));
			}
		}
		//sort links by distance
		std::sort(distances.begin(), distances.end(), [](const distancePair& a, const distancePair& b) {
			return a.first < b.first;
		});
		int last1, last2;
		connectAll(points, distances, regions, idToRegions, last1, last2);
		long long score = points.at(last1).x * points.at(last2).x;
		std::cout << score << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/