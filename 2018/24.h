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

//battle simulator with affinities

enum struct Effect {
	COLD = 0,
	FIRE = 1,
	BLUDGEONING = 2,
	SLASHING = 3,
	RADIATION = 4
};
enum struct Affinity {
	NORMAL = 0,
	WEAK = 1,
	IMMUNE = 2,
};

constexpr Effect EffectFromString(std::string effect) {
	if (effect == "cold") return Effect::COLD;
	else if (effect == "fire") return Effect::FIRE;
	else if (effect == "bludgeoning") return Effect::BLUDGEONING;
	else if (effect == "slashing") return Effect::SLASHING;
	else if (effect == "radiation") return Effect::RADIATION;
}
constexpr Affinity AffinityFromString(std::string affinity) {
	if (affinity == "weak") return Affinity::WEAK;
	else if (affinity == "immune") return Affinity::IMMUNE;
	return Affinity::NORMAL;
}

struct Army {
	bool virusSide = false;
	int nUnits;
	int hp;
	std::unordered_map<Effect, Affinity> affinities;
	int damage;
	Effect effect;
	int priority;
	int target = -1;
	int power() const {return nUnits * damage;}
	int calculateDamage(int pwr, Effect effect) {
		if (affinities[effect] == Affinity::NORMAL) {
			return pwr;
		}
		if (affinities[effect] == Affinity::WEAK) {
			return (pwr*2);
		}
		return 0;
	}
	void takeDamage(int pwr, Effect eff) {
		int lost = calculateDamage(pwr, eff)/hp;
		nUnits -= lost;
		// std::cout << "Army " << priority << " lost " << lost << " units" << std::endl;
	}
};

const std::regex pattern (R"((\d+) units each with (\d+) hit points (?:\((.+)\) )?with an attack that does (\d+) (\w+) damage at initiative (\d+))", std::regex_constants::optimize);
struct Task
{
	static void Simulate(std::vector<Army>& armies) {
		int virusArmies = 0, systemArmies = 0;
		for (auto& army : armies) {
			if (army.virusSide) virusArmies++;
			else systemArmies++;
		}
		std::sort(armies.begin(), armies.end(), [](const Army& a, const Army& b) { return a.priority > b.priority; });
		while (virusArmies > 0 && systemArmies > 0) {
			std::vector<int> pickOrder;
			for (int i=0; i<armies.size(); i++) {
				pickOrder.push_back(i);
			}
			std::vector<int> unselected = pickOrder;
			std::sort(pickOrder.begin(), pickOrder.end(), [&armies](int a, int b) {
				if (armies[a].power() != armies[b].power()) return armies[a].power() > armies[b].power();
				return armies[a].priority > armies[b].priority;
			});
			for (int i : pickOrder) {
				Army& atkArmy = armies[i];
				// std::cout << atkArmy.power() << std::endl;
				int targetID = -1;
				for (int j=0; j < armies.size(); j++) {
					Army& candidate = armies[j];
					if (i == j) continue;
					int damage = candidate.calculateDamage(atkArmy.power(), atkArmy.effect);
					if (damage == 0) continue;
					if (candidate.virusSide == atkArmy.virusSide) continue;
					if (std::find(unselected.begin(), unselected.end(), j) == unselected.end()) continue;
					if (targetID == -1) {
						targetID = j;
						continue;
					}
					const auto targetPred = [&atkArmy](Army& a, Army& b) {
						int aDmg = a.calculateDamage(atkArmy.power(), atkArmy.effect);
						int bDmg = b.calculateDamage(atkArmy.power(), atkArmy.effect);
						if (aDmg != bDmg) return aDmg > bDmg;
						if (a.power() != b.power()) return a.power() > b.power();
						return a.priority > b.priority;
					};
					if (targetPred(candidate, armies[targetID])) {
						targetID = j;
					}
				}
				atkArmy.target = targetID;
				if (targetID != -1)
					unselected.erase(std::find(unselected.begin(), unselected.end(), targetID));
			}
			int turns = 0;
			for (Army& army : armies) {
				if (army.target == -1 || army.nUnits <= 0) continue;
				Army& target = armies[army.target];
				target.takeDamage(army.power(), army.effect);
				turns++;
			}
			if (turns == 0) {
				std::cout << "stale!" << std::endl;
				return;
			}
			for (int i = armies.size()-1; i >= 0; i--) {
				if (armies[i].nUnits <= 0) {
					if (armies[i].virusSide) virusArmies--;
					else systemArmies--;
					armies.erase(armies.begin() + i);
				}
			}
		}
	}

	static inline std::vector<Army> backup;
	static void run() {

		std::ifstream inputStream("2018/24.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		std::smatch match;
		bool virusSide = false;
		while (std::getline(inputStream, line))
		{
			if (line.empty()) {
				virusSide = true;
				continue;
			}
			if (regex_match(line, match, pattern)) {
				Army army;
				army.nUnits = std::stoi(match[1].str());
				army.hp = std::stoi(match[2].str());
				std::string affinities = match[3].str();
				army.damage = std::stoi(match[4].str());
				army.effect = EffectFromString(match[5].str());
				army.priority = std::stoi(match[6].str());
				std::vector<std::string> sections = Utils::split(affinities, ";");
				for (auto& str : sections) {
					std::istringstream ss(str);
					std::string key;
					ss >> key;
					ss.ignore(3);
					std::string effect;
					while (ss >> effect) {
						if (effect.back() == ',') effect.pop_back();
						army.affinities[EffectFromString(effect)] = AffinityFromString(key);
					}
				}
				if (virusSide) army.virusSide = true;
				backup.push_back(army);
			}
		}
		inputStream.close();
		std::vector<Army> armies = backup;
		Simulate(armies);
		int cnt = 0;
		for (auto& army : armies) {
			cnt += army.nUnits;
		}
		std::cout << cnt << std::endl;

	}
	static void runPart2() {
		int boost = 1;
		while (true) {
			std::vector<Army> armies = backup;
			for (auto& army : armies) {
				if (!army.virusSide) {
					army.damage += boost;
				}
			}
			Simulate(armies);
			bool allSystem = true;
			for (auto& army : armies) {
				if (army.virusSide) {
					allSystem = false;
				}
			}
			if (allSystem) {

				int cnt = 0;
				for (auto& army : armies) {
					cnt += army.nUnits;
				}
				std::cout << cnt << std::endl;
				std::cout << boost << std::endl;
				return;
			}
			boost++;
		}
	}
};

//-------------- NOTES AREA ----------------
/*

*/