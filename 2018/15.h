#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "GridBase.h"
#include "utils.h"

//mmo rpg (its complicated)

enum struct Side {
	ELF,
	GOBLIN
};
struct Entity {
	Side side;
	int x, y;
	int hp;

	int distance(Entity& other) {
		return abs(x - other.x) + abs(y - other.y);
	}
};

struct EntityFactory {
	static Entity CreateElf(int x, int y) {
		return Entity{Side::ELF, x, y, 200 };
	}
	static Entity CreateGoblin(int x, int y) {
		return Entity{Side::GOBLIN, x, y, 200 };
	}
};

struct SimulationRes {
	Side side;
	std::vector<Entity> remainingEntities;
	int rounds;
};
struct Task
{
	struct DijkstraCell {
		int w;
		int firstMoveX = 0;
		int firstMoveY = 0;
		bool target = false;
		bool operator <(const DijkstraCell& other) const {
			if (w != other.w) return w < other.w;
			if (firstMoveX != other.firstMoveY) return firstMoveY < other.firstMoveY;
			return firstMoveX < other.firstMoveX;
		}
	};
	struct DijkstraNode {
		int x,y;
		DijkstraCell info;
		bool operator <(const DijkstraNode& other) const {
			if (info.w != other.info.w) return info.w < other.info.w;
			if (y != other.y) return y < other.y;
			if (x != other.x) return x < other.x;
			if (info.firstMoveY != other.info.firstMoveY) return info.firstMoveY < other.info.firstMoveY;
			return info.firstMoveX < other.info.firstMoveX;
		}
		bool operator==(const DijkstraNode & other) const {
			return x == other.x && y == other.y;
		}
	};

	static constexpr int MAX_INT = std::numeric_limits<int>::max();

	static SimulationRes LaunchSimulation(DynamicGrid<char>& map,
		std::vector<Entity> elfs,
		std::vector<Entity> goblins,
		int elfAtk,
		int goblinAtk) {
		SimulationRes res;
		std::vector<std::pair<int,Side>> orderList;
		auto bookOrderEntity = [](const Entity& e1, const Entity& e2) {
			if (e1.y != e2.y) return e1.y < e2.y;
			return e1.x < e2.x;
		};
		auto buildOrder = [&]() {
			orderList.clear();
			orderList.reserve(elfs.size() + goblins.size());
			for (int i=0; i<elfs.size(); i++) {
				orderList.emplace_back(i,Side::ELF);
			}
			for (int i=0; i<goblins.size(); i++) {
				orderList.emplace_back(i,Side::GOBLIN);
			}
			std::sort(orderList.begin(), orderList.end(), [&goblins, &elfs, &bookOrderEntity](std::pair<int,Side>& i1, std::pair<int,Side>& i2) {
				Entity& e1 = i1.second == Side::ELF ? elfs[i1.first] : goblins[i1.first];
				Entity& e2 = i2.second == Side::ELF ? elfs[i2.first] : goblins[i2.first];
				return bookOrderEntity(e1, e2);
			});
		};
		//x y w

		auto pickNextPos = [&map](const Entity& entity, const std::vector<Entity>& targets)
		-> std::optional<std::pair<int,int>>
		{
			//already has enemy in range
			if (map.countNeighbours4(entity.x, entity.y, [&entity](char ch) {
				return ch == (entity.side == Side::ELF ? 'G' : 'E');
			}) > 0) return std::nullopt;

			//get possible destinations
			std::vector<std::pair<int,int>> destinations;
			for (auto& target : targets) {
				for (auto& pair : map.getNeighbourCoords4(target.x, target.y)) {
					if (map.get(pair.first, pair.second) == '.') {
						destinations.push_back(pair);
					}
				}
			};

			//----------- pathfinding --------------
			//init weights
			DynamicGrid<DijkstraCell> weights;
			weights.resize(map.getWidth(), map.getHeight());
			map.foreachCoord([&](int x, int y) {
				if (map.get(x,y) != '.') {
					weights.set(x,y,{-1, MAX_INT, MAX_INT});
				}
				else {
					weights.set(x,y,{MAX_INT, MAX_INT, MAX_INT});
				}
			});
			for (auto& [x, y] : destinations) {
				weights.getRef(x,y).target = true;
			}

			//create queue
			std::deque<DijkstraNode> queue;
			auto queueItem = [&queue](DijkstraNode node) {
				bool found = false;
				for (int i=0; i<queue.size(); i++) {
					if (node < queue[i]) {
						queue.insert(queue.begin() + i, node);
						found = true;
						break;
					}
				}
				if (!found) {
					queue.push_back(node);
				}
			};
			for (auto& [x, y] : map.getNeighbourCoords4(entity.x, entity.y)) {
				if (map.get(x,y) == '.') {
					queueItem(DijkstraNode{x, y, {1, x, y, weights.get(x,y).target}});
				}
			}
			while (!queue.empty()) {
				DijkstraNode current = queue.front();
				queue.pop_front();
				if (current.info.target)
					return {{current.info.firstMoveX, current.info.firstMoveY}};
				for (auto& [x,y] : map.getNeighbourCoords4(current.x, current.y)) {
					const DijkstraCell& candidate = weights.getRef(x, y);
					if (candidate.w == -1) continue;
					DijkstraCell cell = current.info;
					cell.w++;
					if (candidate.target) {
						cell.target = true;
					}
					if (cell < weights.get(x,y)) {
						weights.set(x,y,cell);
						queueItem(DijkstraNode{x, y, cell});
					}
				}
			}
			return std::nullopt;
		};
		auto move = [&](Entity& entity, std::pair<int, int>& pos) {
			map.set(entity.x, entity.y, '.');
			map.set(pos.first, pos.second, entity.side == Side::ELF ? 'E' : 'G');
			entity.x = pos.first;
			entity.y = pos.second;
		};
		auto attack = [&](Entity& entity, std::vector<Entity>& targets) -> std::optional<int> {
			int bestTarget = -1;
			for (int i=0; i<targets.size(); i++) {
				if (targets[i].distance(entity) == 1) {
					if (bestTarget < 0 ||
						targets[i].hp < targets[bestTarget].hp ||
						targets[i].hp == targets[bestTarget].hp && bookOrderEntity(targets[i], targets[bestTarget]))
						bestTarget = i;
				}
			}
			if (bestTarget == -1) return std::nullopt;
			Entity& picked = targets[bestTarget];
			picked.hp -= (entity.side == Side::ELF ? elfAtk : goblinAtk);
			if (picked.hp <= 0) return bestTarget;
			return std::nullopt;
		};
		int score = 0;
		int rounds = 0;
		while (true) {
			buildOrder();
			bool endGame = false;
			for (int i=0; i<orderList.size(); i++) {
				auto side = orderList[i].second;
				auto enemySide = side == Side::ELF ? Side::GOBLIN : Side::ELF;
				auto& allies = (side == Side::ELF ? elfs : goblins);
				auto& enemies = (side == Side::ELF ? goblins : elfs);
				Entity& e = allies[orderList[i].first];
				auto nextPos = pickNextPos(e, enemies);
				if (nextPos)
					move(e, nextPos.value());
				auto toRemove = attack(e, enemies);
				if (toRemove) {
					int removed = toRemove.value();
					map.set(enemies[removed].x, enemies[removed].y, '.');
					enemies.erase(enemies.begin() + removed);
					int orderID = std::find(orderList.begin(), orderList.end(), std::make_pair(removed, enemySide)) - orderList.begin();
					orderList.erase(orderList.begin() + orderID);
					for (int o=orderList.size()-1; o>=0; o--) {
						if (orderList[o].second == side) continue;
						if (orderList[o].first > removed) {
							orderList[o].first--;
						}
					}
					if (orderID < i) i--;

					//win condition
					if (enemies.empty()) {
						score = 0;
						res.remainingEntities = allies;
						endGame = true;
						if (i==orderList.size() - 1) rounds++;
						res.rounds = rounds;
						res.side = side;
						break;
					}
				}
			}
			if (endGame) {
				break;
			}
			rounds++;
		}
		score *= rounds;
		return res;
	}


	static inline DynamicGrid<char> init;
	static inline std::vector<Entity> elfs;
	static inline std::vector<Entity> goblins;
	static void run() {

		std::ifstream inputStream("2018/15.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		DynamicGrid<char> map;
		std::string line;
		{
			int y = 0;
			while (std::getline(inputStream, line))
			{
				for (int x = 0; x < line.size(); x++) {
					init.add(x,y,line[x]);
					if (line[x] == 'G') {
						goblins.push_back(EntityFactory::CreateGoblin(x,y));
					}
					else if (line[x] == 'E') {
						elfs.push_back(EntityFactory::CreateElf(x,y));
					}
				}
				y++;
			}
		}
		inputStream.close();
		map = init;
		auto res = LaunchSimulation(map, elfs, goblins, 3, 3);
		int hp = 0;
		for (auto& unit : res.remainingEntities) {
			hp += unit.hp;
		}
		map.print([](char _){return _;});
		std::cout << res.rounds * hp << "=" << res.rounds << "*" << hp << std::endl;
	}
	static void runPart2() {
		for (int i=4;; i++) {
			DynamicGrid<char> map = init;
			auto res = LaunchSimulation(map, elfs, goblins, i, 3);
			if (res.side == Side::ELF && res.remainingEntities.size() == elfs.size()) {
				int hp = 0;
				for (auto& unit : res.remainingEntities) {
					hp += unit.hp;
				}
				map.print([](char _){return _;});
				std::cout << res.rounds * hp << "=" << res.rounds << "*" << hp << std::endl;
				std::cout << i << std::endl;
				return;
			}
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/