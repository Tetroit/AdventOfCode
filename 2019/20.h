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

//Donut maze with portals

struct Task
{
	static inline DynamicGrid<int> schematic;
	static inline ivec2 start;
	static inline ivec2 end;
	static void run() {

		std::ifstream inputStream("2019/20.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		DynamicGrid<char> raw;
		std::string line;
		int readY = 0;
		while (std::getline(inputStream, line))
		{
			raw.fillFromLine(readY, line, DefaultConvert<char>());
			readY++;
		}
		inputStream.close();
		schematic.resize(raw.getWidth()-4, raw.getHeight()-4);
		std::unordered_map<uint16_t, std::vector<ivec2>> portals;
		for (int x=2; x< raw.getWidth()-2; x++) {
			for (int y=2; y< raw.getHeight()-2; y++) {
				char val = raw.get(x, y);
				if (val == '.') {
					for (int dir=0; dir<4; dir++) {
						auto [dx, dy] = Utils::UtilFacingVec[dir];
						char neighbour = raw.get(x + dx,y + dy);
						if (std::isupper(neighbour)) {
							char neighbour2 = raw.get(x+dx+dx,y+dy+dy);
							uint16_t key = 0;
							switch (dir) {
								case 0:
								case 3:
									key = static_cast<uint16_t>(neighbour) << 8 | static_cast<uint16_t>(neighbour2); break;
								case 1:
								case 2:
									key = static_cast<uint16_t>(neighbour2) << 8 | static_cast<uint16_t>(neighbour); break;
							}
							portals[key].emplace_back(x-2, y-2);
						}
					}
					schematic.set(x-2, y-2, 0);
				}
				else
					schematic.set(x-2, y-2, -1);
			}
		}
		uint16_t startKey = (uint16_t)'A' << 8 | (uint16_t)'A';
		uint16_t endKey = (uint16_t)'Z' << 8 | (uint16_t)'Z';
		start = portals[startKey].front();
		end = portals[endKey].back();
		portals.erase(startKey);
		portals.erase(endKey);
		for (auto& [key, portalPair] : portals) {
			schematic.set(portalPair[0].x, portalPair[0].y, portalPair[1].x << 16 | portalPair[1].y);
			schematic.set(portalPair[1].x, portalPair[1].y, portalPair[0].x << 16 | portalPair[0].y);
		}

		schematic.print([](int val) {
			if (val == 0) return '.';
			if (val == -1) return '#';
			return '0';
		});

		//just a recycled dijkstra from GridBase.h
		int score = -1;
		using weight_t = int;

        std::vector<std::vector<weight_t>> weights;
        weights.resize(schematic.getHeight());
        for (int y = 0; y < schematic.getHeight(); ++y) {
            weights[y].resize(schematic.getWidth());
            for (int x = 0; x < schematic.getWidth(); ++x) {
                weights[y][x] = schematic.get(x,y) == -1 ? -1 : INT_MAX;
            }
        }
        std::vector<std::tuple<weight_t, int, int>> queue;

        auto getWeight = [&](int x, int y) -> int {
            return weights[y][x];
        };
        auto setWeight = [&](int x, int y, weight_t weight) -> void {
            weights[y][x] = weight;
        };
        setWeight(start.x, start.y, 0);
        queue.emplace_back(0, start.x, start.y);
        while (!queue.empty()) {
            auto [val, x, y] = queue.front();
            if (x == end.x && y == end.y) {
                score = val;
            	break;
            }
            std::vector<std::tuple<int, int, int>> toAdd;

            for (auto[nx, ny] : schematic.getNeighbourCoords4(x, y)) {

                if (schematic.get(nx, ny) == -1)
                    continue;

                int weight = val + 1;
                if (getWeight(nx, ny) > weight) {
                    setWeight(nx, ny, weight);
                    toAdd.emplace_back(weight, nx, ny);
                }
            }
        	auto cell = schematic.get(x, y);
        	if (cell > 0) {
        		int nx = cell >> 16;
        		int ny = cell & 0xFFFF;
        		int weight = val + 1;
        		if (getWeight(nx, ny) > weight) {
        			setWeight(nx, ny, weight);
        			toAdd.emplace_back(weight, nx, ny);
        		}
        	}

            queue.erase(queue.begin());

            for (auto& t : toAdd) {
                int weight = std::get<0>(t);
                bool inserted = false;
                for (auto it = queue.begin(); it<queue.end(); it++) {
                    if (weight < std::get<0>(*it)) {
                        queue.insert(it, t);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    queue.emplace_back(t);
                }
            }
        }
		std::cout << score << "\n";
	}
	static bool isInner(int x, int y, int w, int h) {
		if (x == 0 || y == 0 || x == w-1 || y == h-1) return false;
		return true;
	}
	static void runPart2() {

		//just a recycled dijkstra from GridBase.h
		int score = -1;
		using weight_t = int;

        std::vector<std::vector<std::vector<weight_t>>> weights{};
		auto createNewLayer = [&]() {
			weights.emplace_back();
			auto& newLayer = weights.back();
			newLayer.resize(schematic.getHeight());
			for (int y = 0; y < schematic.getHeight(); ++y) {
				newLayer[y].resize(schematic.getWidth());
				for (int x = 0; x < schematic.getWidth(); ++x) {
					newLayer[y][x] = schematic.get(x,y) == -1 ? -1 : INT_MAX;
				}
			}
		};
		createNewLayer();
		//layer, weight, x, y
        std::vector<std::tuple<weight_t, int, int, int>> queue;

        auto getWeight = [&](int l, int x, int y) -> int {
            return weights[l][y][x];
        };
        auto setWeight = [&](int l, int x, int y, weight_t weight) -> void {
        	weights[l][y][x] = weight;
        };
        setWeight(0, start.x, start.y, 0);
        queue.emplace_back(0, 0, start.x, start.y);
        while (!queue.empty()) {
            auto [val, layer, x, y] = queue.front();
            if (x == end.x && y == end.y && layer == 0) {
                score = val;
            	break;
            }
            std::vector<std::tuple<int, weight_t, int, int>> toAdd;

            for (auto[nx, ny] : schematic.getNeighbourCoords4(x, y)) {

                if (schematic.get(nx, ny) == -1)
                    continue;

                int weight = val + 1;
                if (getWeight(layer, nx, ny) > weight) {
                    setWeight(layer, nx, ny, weight);
                    toAdd.emplace_back(weight, layer, nx, ny);
                }
            }
        	auto cell = schematic.get(x, y);
        	if (cell > 0) {
        		int nx = cell >> 16;
        		int ny = cell & 0xFFFF;
        		int weight = val + 1;
        		int nl = isInner(x, y, schematic.getWidth(), schematic.getHeight()) ? layer+1 : layer-1;
        		if (nl >= 0) {
        			if (nl == weights.size()) createNewLayer();
        			if (getWeight(nl, nx, ny) > weight) {
        				setWeight(nl, nx, ny, weight);
        				toAdd.emplace_back(weight, nl, nx, ny);
        			}
        		}
        	}

            queue.erase(queue.begin());

            for (auto& t : toAdd) {
                int weight = std::get<0>(t);
                bool inserted = false;
                for (auto it = queue.begin(); it<queue.end(); it++) {
                    if (weight < std::get<0>(*it)) {
                        queue.insert(it, t);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    queue.emplace_back(t);
                }
            }
        }
		std::cout << score << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*

*/