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
#include "vec.h"

//feature matching

struct Task
{
	static inline std::vector<std::vector<ivec3>> scans;
	static inline std::vector<ivec3> scanners;

	enum struct Axis {
		X = 0,
		Y = 1,
		Z = 2,
		NX = 3,
		NY = 4,
		NZ = 5,
		ERR = 1000
	};
	static ivec3 getOrth(Axis axis) {
		switch (axis) {
			case Axis::X: return {1,0,0};
			case Axis::Y: return {0,1,0};
			case Axis::Z: return {0,0,1};
			case Axis::NX: return {-1,0,0};
			case Axis::NY: return {0,-1,0};
			case Axis::NZ: return {0,0,-1};
		}
	}
	static Axis axisCross(Axis ax1, Axis ax2) {
		auto orth1 = getOrth(ax1);
		auto orth2 = getOrth(ax2);
		ivec3 cross;
		cross.x = (orth1.y * orth2.z) - (orth1.z * orth2.y);
		cross.y = (orth1.z * orth2.x) - (orth1.x * orth2.z);
		cross.z = (orth1.x * orth2.y) - (orth1.y * orth2.x);
		if (cross.x == 1) return Axis::X;
		if (cross.y == 1) return Axis::Y;
		if (cross.z == 1) return Axis::Z;
		if (cross.x == -1) return Axis::NX;
		if (cross.y == -1) return Axis::NY;
		if (cross.z == -1) return Axis::NZ;
		return Axis::ERR;
	}

	static int getValueAlongAxis(Axis axis, ivec3 pos) {
		switch (axis) {
			case Axis::X: return pos.x;
			case Axis::Y: return pos.y;
			case Axis::Z: return pos.z;
			case Axis::NX: return -pos.x;
			case Axis::NY: return -pos.y;
			case Axis::NZ: return -pos.z;
			default: return 0;
		}
	}
	static void setValueAlongAxis(Axis axis, ivec3& pos, int value) {
		switch (axis) {
			case Axis::X: pos.x = value; break;
			case Axis::Y: pos.y = value; break;
			case Axis::Z: pos.z = value; break;
			case Axis::NX: pos.x = -value; break;
			case Axis::NY: pos.y = -value; break;
			case Axis::NZ: pos.z = -value; break;
			default: break;
		}
	}

	struct Transform {
		Axis localX;
		Axis localY;
		Axis localZ;

		ivec3 localToGlobal(ivec3 point) {
			ivec3 res;
			setValueAlongAxis(localX, res, point.x);
			setValueAlongAxis(localY, res, point.y);
			setValueAlongAxis(localZ, res, point.z);
			return res;
		}
		ivec3 globalToLocal(ivec3 point) {
			ivec3 res;
			res.x = getValueAlongAxis(localX, point);
			res.y = getValueAlongAxis(localY, point);
			res.z = getValueAlongAxis(localZ, point);
			return res;
		}
	};

	static DynamicGrid<ivec3> scanGraph(const std::vector<ivec3>& scan) {

		DynamicGrid<ivec3> res;
		res.resize(scan.size(), scan.size());
		for (int i=0; i<scan.size()-1; i++) {
			for (int j=i+1; j<scan.size(); j++) {
				ivec3 d = scan[i] - scan[j];
				res.set(j,i,d);
				res.set(i,j,-d);
			}
		}
		return res;
	};
	static inline std::vector<Transform> transforms;

	static bool matchScans(
		const std::vector<ivec3>& scan1,
		const std::vector<ivec3>& scan2,
		int matchThreshold,
		ivec3& outScan2Pos,
		Transform& outTransform,
		std::unordered_map<int, int>& matches) {

		DynamicGrid<ivec3> scan1graph = scanGraph(scan1);
		DynamicGrid<ivec3> scan2graph = scanGraph(scan2);

		for (int v1 = 0; v1 < scan1.size() - matchThreshold; v1++) {
			for (int v2 = 0; v2 < scan2.size(); v2++) {
				for (auto& transform : transforms) {
					matches.clear();
					int nMatches = 0;
					for (int o1 = 0; o1 < scan1.size(); o1++) {
						if (v1 == o1) continue;
						for (int o2 = 0; o2 < scan2.size(); o2++) {
							if (o2 == v2) continue;
							if (scan1graph.get(v1, o1) == transform.localToGlobal(scan2graph.get(v2, o2))) {
								matches.emplace(o2, o1);
								nMatches++;
							}
						}
					}
					if (nMatches >= matchThreshold - 1) {
						matches.emplace(v2,v1);
						outTransform = transform;
						outScan2Pos = scan1.at(v1) - transform.localToGlobal(scan2.at(v2));
						return true;
					}
				}
			}
		}
		return false;
	}

	static bool matchScansV2(
		const std::vector<ivec3>& scan1,
		const std::vector<ivec3>& scan2,
		int matchThreshold,
		ivec3& outScan2Pos,
		Transform& outTransform) {

		DynamicGrid<ivec3> scan1graph = scanGraph(scan1);
		DynamicGrid<ivec3> scan2graph = scanGraph(scan2);

		std::unordered_map<ivec3, int, ivec3hash> occurrences;
		for (auto& transform : transforms) {
			occurrences.clear();
			for (auto v1 : scan1) {
				for (auto v2 : scan2) {
					outScan2Pos = v1 - transform.localToGlobal(v2);
					occurrences[outScan2Pos]++;
					if (occurrences[outScan2Pos] >= matchThreshold) {
						outTransform = transform;
						return true;
					}
				}
			}
		}
		return false;
	}
	static void run() {

		std::ifstream inputStream("2021/19.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) continue;
			std::istringstream iss(line);
			if (line.substr(0,3) == "---") {
				scans.emplace_back();
				continue;
			}
			ivec3 coords;
			char shit;
			iss>>coords.x>>shit>>coords.y>>shit>>coords.z;
			scans.back().emplace_back(coords);
		}
		inputStream.close();

		for (int x=0; x<6; x++) {
			for (int y=0; y<6; y++) {
				if (x%3 == y%3) continue;
				Transform transform;
				transform.localX = (Axis)x;
				transform.localY = (Axis)y;
				transform.localZ = axisCross(transform.localX, transform.localY);
				transforms.emplace_back(transform);
			}
		}

		std::unordered_set<int> matched {0};
		std::unordered_set<int> tested {};
		scanners.resize(scans.size());
		scanners[0] = ivec3(0,0,0);

		std::vector<ivec3> globalScan = scans[0];

		for (int it = 0; it < scans.size(); it++) {
			if (scans.size() == matched.size()) break;
			for (int srcId : matched) {
				std::vector<int> toAdd;
				if (tested.contains(srcId)) continue;
				for (int i=0; i<scans.size(); i++) {
					if (matched.contains(i)) continue;

					ivec3 scanPos;
					Transform scanTransform{};
					if (matchScansV2(scans[srcId], scans[i], 12, scanPos, scanTransform)) {
						scanners[i] = scanPos;
						std::cout << "matched scan " << srcId << " with " << i << " at coord " << scanners[i].x << ", " << scanners[i].y << ", " << scanners[i].z << std::endl;
						//
						// for (const auto& [key, val] : matches) {
						// 	auto& vec = scans[srcId][key];
						// 	std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
						// }

						for (auto& point : scans[i]) {
							point = scanTransform.localToGlobal(point) + scanPos;
							globalScan.emplace_back(point);
						}
						toAdd.emplace_back(i);
					}
				}
				for (auto& scanId : toAdd) {
					matched.emplace(scanId);
				}
			}
		}
		std::sort(globalScan.begin(), globalScan.end(), [](const ivec3& a, const ivec3& b) {
			if (a.x != b.x) return a.x < b.x;
			if (a.y != b.y) return a.y < b.y;
			return a.z < b.z;
		});
		globalScan.erase(std::unique(globalScan.begin(), globalScan.end()), globalScan.end());
		std::cout << globalScan.size() << std::endl;
		//551 high
	}
	static void runPart2() {
		int maxDist = 0;
		for (int i=0; i<scanners.size()-1; i++) {
			for (int j=i+1; j<scanners.size(); j++) {
				int dist = (scanners[i] - scanners[j]).len();
				if (dist > maxDist) maxDist = dist;
			}
		}
		std::cout << maxDist << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

oh no.. my mother did not prepare me for 7 nested for loops
ahhh okay there is a trick to it

*/