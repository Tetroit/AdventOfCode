#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

//magic battle sim

struct BattleState {
	int bossHp;
	int bossAttack;
	int playerMana;
	int playerHp;
	int playerArmor;
};

struct Spell {
	int mana;
	int duration;
	//not my proudest choice to use functors but oh well too late
	std::function<void(BattleState&)> effect;
	std::function<void(BattleState&)> effectOn;
	std::function<void(BattleState&)> effectOff;
	Spell(int _mana, int _duration,
		std::function<void(BattleState&)> on = {} ,
		std::function<void(BattleState&)> off = {} ,
		std::function<void(BattleState&)> update = {} ) : mana(_mana), duration(_duration),
	effectOn(std::move(on)), effectOff(std::move(off)), effect(std::move(update)){

	}
	~Spell() = default;
	bool Start(BattleState& state) {
		if (effectOn) effectOn(state);
		if (duration <= 0) {
			if (effectOff) effectOff(state);
			return false;
		}
		return true;
	}
	bool Apply(BattleState& state) {
		if (duration <= 0) {
			if (effectOff) effectOff(state);
			return false;
		}
		if (effect) effect(state);
		duration--;
		if (duration <= 0) {
			if (effectOff) effectOff(state);
			return false;
		}
		return true;
	}
};

enum SpellType {
	MISSILE = 0,
	DRAIN = 1,
	SHIELD = 2,
	POISON = 3,
	RECHARGE = 4
};

struct Task
{
	static Spell CreateSpell(SpellType type) {
		switch (type) {
			case MISSILE: {
				return Spell(53, 0, [](BattleState& state){
					state.bossHp -= 4;
				});
				break;
			}
			case DRAIN: {
				return Spell(73, 0, [](BattleState& state){
					state.bossHp -= 2;
					state.playerHp -= 2;
				});
				break;
			}
			case SHIELD: {
				return Spell(113, 6,
					[](BattleState& state){
						state.playerArmor += 7;
					},
					[](BattleState& state){
						state.playerArmor -= 7;
					}
				);
				break;
			}
			case POISON: {
				return Spell(173, 6, {},{}, [](BattleState& state){
					state.bossHp -= 3;
				});
				break;
			}
			case RECHARGE: {
				return Spell(229, 5, {},{},[](BattleState& state){
					state.playerMana += 101;
				});
				break;
			}
		}
	}
	static int LeastMana(BattleState state, bool hard) {
		int minMana = std::numeric_limits<int>::max();
		std::unordered_map<SpellType, Spell> spells;
		Turn(state, spells, minMana, 0, hard);
		return minMana;
	}
	static bool CheckWin(const BattleState& state) {
		if (state.bossHp <= 0) {
			return true;
		}
		return false;
	}
	static void Turn(BattleState state, std::unordered_map<SpellType, Spell> spells, int& minMana, int spentMana, bool hard) {

		//----------------- Player turn ---------------
		//Play all spells
		std::vector<SpellType> toRemove;
		for (auto& [type, spell] : spells) {
			if (!spell.Apply(state)) {
				toRemove.push_back(type);
			}
		}
		//Remove expired spells
		for (auto& spell : toRemove) {
			spells.erase(spell);
		}
		toRemove.clear();

		if (CheckWin(state)) {
			minMana = std::min(minMana, spentMana);
			return;
		}
		//Play a spell (RECURSIVE)
		for (int i=0; i<5; i++) {
			SpellType type = (SpellType)i;
			if (spells.contains(type))
				continue;
			auto newState = state;
			auto newSpells = spells;

			//Apply new spell
			newSpells.emplace(type, CreateSpell(type));
			auto& newSpell = newSpells.at(type);
			if (newSpell.mana > newState.playerMana)
				continue;
			newState.playerMana -= newSpell.mana;
			auto newSpentMana = spentMana + newSpell.mana;
			if (!newSpell.Start(newState)) {
				newSpells.erase(type);
			}
			if (CheckWin(state)) {
				minMana = std::min(minMana, newSpentMana);
				continue;
			}
			//------------------- Boss turn -------------------
			//Play all spells

			for (auto& [type, spell] : newSpells) {
				if (!spell.Apply(newState)) {
					toRemove.push_back(type);
				}
			}
			//Remove expired spells
			for (auto& spell : toRemove) {
				newSpells.erase(spell);
			}
			toRemove.clear();
			if (CheckWin(newState)) {
				minMana = std::min(minMana, newSpentMana);
				continue;
			}
			newState.playerHp -= std::max(newState.bossAttack - newState.playerArmor, 1);

			if (hard) {
				newState.playerHp--;
			}

			if (newState.playerHp <= 0)
				continue;

			Turn (newState, newSpells, minMana, newSpentMana, hard);
		}
	}
	static void run() {
		BattleState startState{71, 10, 500, 50, 0};
		std::cout << LeastMana(startState, false) << std::endl;
	}
	static void runPart2() {
		BattleState startState{71, 10, 500, 50, 0};
		std::cout << LeastMana(startState, true) << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/