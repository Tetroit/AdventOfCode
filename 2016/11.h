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

//elevator and generator and microchips

// enum class ElementType {
// 	GENERATOR,
// 	MICROCHIP,
// };
// struct Element {
// 	ElementType type;
// 	int chem;
// 	int uid;
// 	bool operator==(const Element & other) const {
// 		return other.chem == chem && type == other.type;
// 	}
// };
// struct State {
// 	State(int maxFloors = 4) {
// 		elements.resize(maxFloors);
// 		this->maxFloors = maxFloors;
// 	}
// 	std::vector<std::vector<Element>> elements;
// 	std::unordered_map<int, int> elementToFloor;
// 	int elevator;
// 	const std::vector<Element>& GetCurrentFloor() const {
// 		return elements[elevator];
// 	}
// 	void Add(Element&& element, int floor) {
// 		elements[floor].emplace_back(std::move(element));
// 		elementToFloor[element.uid] = floor;
// 	}
// 	void MoveUp(int floor, int n) {
// 		if (floor == maxFloors - 1)
// 			return;
// 		Element toMove = elements[floor][n];
// 		elements[floor+1].push_back(toMove);
// 		elements[floor].erase(elements[floor].begin() + n);
// 		elementToFloor[toMove.uid] = floor+1;
// 	}
// 	void MoveDown(int floor, int n) {
// 		if (floor == 0)
// 			return;
// 		Element toMove = elements[floor][n];
// 		elements[floor-1].push_back(toMove);
// 		elements[floor].erase(elements[floor].begin() + n);
// 		elementToFloor[toMove.uid] = floor-1;
// 	}
// 	int MaxFloor() const {
// 		return maxFloors - 1;
// 	}
// 	bool IsFinish() const {
// 		for (auto& [id, floor] : elementToFloor) {
// 			if (floor != maxFloors - 1)
// 				return false;
// 		}
// 		return true;
// 	}
// 	bool operator==(const State & other) const {
// 		for (auto& [uid, floor] : elementToFloor) {
// 			if (elevator != other.elevator) {
// 				return false;
// 			}
// 			if (other.elementToFloor.at(uid) != floor) {
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
//
// private:
// 	int maxFloors;
// };
struct StateOpt {
	std::array<int, 4> f{0,0,0,0};
	int elev = 0;
	void Add(int id, bool generator, int floor) {
		f[floor] |= (1 << (id + id + generator));
	}
	void Remove(int id, bool generator, int floor) {
		f[floor] &= ~(1 << (id + id + generator));
	}
	void MoveUp(int id, bool g, int floor) {
		Add(id, g, floor+1);
		Remove(id, g, floor);
	}
	void MoveUp(int id, bool g, int id2, bool g2, int floor) {
		Add(id, g, floor+1);
		Remove(id, g, floor);
		Add(id2, g2, floor+1);
		Remove(id2, g2, floor);
	}
	void MoveDown(int id, bool g, int floor) {
		Add(id, g, floor-1);
		Remove(id, g, floor);
	}
	void MoveDown(int id, bool g, int id2, bool g2, int floor) {
		Add(id, g, floor-1);
		Remove(id, g, floor);
		Add(id2, g2, floor-1);
		Remove(id2, g2, floor);
	}
	const int& Here() const {
		return f.at(elev);
	}
	bool operator==(const StateOpt& other) const {
		for (int i=0; i<4; i++) {
			if (f[i] != other.f[i]) {
				return false;
			}
		}
		return elev == other.elev;
	}
	[[nodiscard]] bool IsFinished() const {
		return f[0] == 0 && f[1] == 0 && f[2] == 0;
	}
	[[nodiscard]] bool CheckSafety(int floor) const {
		int cpy = f[floor];
		bool freeChips = false;
		bool hasGenerator = false;
		while (cpy!=0) {
			if ((cpy & 0b11) == 1) {
				freeChips = true;
			}
			if (!hasGenerator && (cpy & 0b10) == 2) {
				hasGenerator = true;
			}
			if (freeChips && hasGenerator)
				return false;
			cpy >>= 2;
		}
		return true;
	}
	void Sort() {
		std::unordered_map<int, int> map;
		int filled = 0;
		for (int floor : f) {
			int i = 0;
			while (floor != 0) {
				if ((floor & 0b11) != 0) {
					if (map.try_emplace(i, filled).second)
						filled++;
				}
				i++;
				floor >>= 2;
			}
		}
		for (int i=0; i<4; i++) {
			int newFloor = 0;
			int oldFloor = f.at(i);
			int it = 0;
			while (oldFloor != 0) {
				newFloor |= (oldFloor & 0b11) << (map.at(it) * 2);
				it++;
				oldFloor >>= 2;
			}
			f[i] = newFloor;
		}
	}
};

struct StateOptHash {
	size_t operator()(const StateOpt& obj) const {
		std::hash<int> hasher;
		return
		hasher(obj.f.at(0)) ^
		hasher(obj.f.at(1)) ^
		hasher(obj.f.at(2)) ^
		hasher(obj.f.at(3)) ^
		hasher(obj.elev);
	}
};
struct Task
{
	// static bool CheckSafety(const std::vector<Element>& state) {
	// 	bool hasGenerator = false;
	// 	std::unordered_map<int, int> status;
	// 	for (auto i : state) {
	// 		if (i.type == ElementType::GENERATOR) {
	// 			hasGenerator = true;
	// 			status[i.chem] |= 1;
	// 		}
	// 		else {
	// 			status[i.chem] |= 2;
	// 		}
	// 	}
	// 	if (!hasGenerator) {
	// 		return true;
	// 	}
	// 	for (auto& [id, val] : status) {
	// 		if (val == 2)
	// 			return false;
	// 	}
	// 	return true;
	// }
	// static int RunCombos(State state) {
	// 	std::vector<State> states{std::move(state)};
	// 	std::vector<int> path{0};
	// 	int fastestFinish = -1;
	// 	int stateN = 0;
	// 	while (true) {
	// 		if (fastestFinish != -1)
	// 			return fastestFinish;
	// 		BranchStates(states[stateN], path[stateN], states, path, fastestFinish);
	// 		stateN++;
	// 	}
	// }
	// static void BranchStates(State state, int step, std::vector<State>& states, std::vector<int>& path, int& fastestFinish) {
	// 	auto checkPlaceState = [&](State&& _state, int _step) -> bool {
	//
	// 		if (_state.IsFinish()) {
	// 			fastestFinish = _step;
	// 			return true;
	// 		}
	// 		if (std::ranges::find(states, _state) == states.end()) {
	// 			states.emplace_back(std::move(_state));
	// 			path.push_back(_step);
	// 		}
	// 		return false;
	// 	};
	// 	for (int i=0; i<state.GetCurrentFloor().size(); i++) {
	// 		if (state.elevator < state.MaxFloor()) {
	// 			{
	// 				State newState = state;
	// 				newState.MoveUp(state.elevator, i);
	// 				newState.elevator++;
	// 				if (CheckSafety(newState.GetCurrentFloor()) && CheckSafety(newState.elements[newState.elevator-1])) {
	// 					if (checkPlaceState(std::move(newState), step+1)) return;
	// 				}
	// 			}
	// 			for (int j=i+1; j<state.GetCurrentFloor().size(); j++) {
	// 				State newState = state;
	// 				newState.MoveUp(state.elevator, j);
	// 				newState.MoveUp(state.elevator, i);
	// 				newState.elevator++;
	// 				if (CheckSafety(newState.GetCurrentFloor()) && CheckSafety(newState.elements[newState.elevator-1])) {
	// 					if (checkPlaceState(std::move(newState), step+1)) return;
	// 				}
	// 			}
	// 		}
	// 		if (state.elevator > 0) {
	// 			{
	// 				State newState = state;
	// 				newState.MoveDown(state.elevator, i);
	// 				newState.elevator--;
	// 				if (CheckSafety(newState.GetCurrentFloor()) && CheckSafety(newState.elements[newState.elevator+1])) {
	// 					if (checkPlaceState(std::move(newState), step+1)) return;
	// 				}
	// 			}
	// 			for (int j=i+1; j<state.GetCurrentFloor().size(); j++) {
	// 				State newState = state;
	// 				newState.MoveDown(state.elevator, j);
	// 				newState.MoveDown(state.elevator, i);
	// 				newState.elevator--;
	// 				if (CheckSafety(newState.GetCurrentFloor()) && CheckSafety(newState.elements[newState.elevator+1])) {
	// 					if (checkPlaceState(std::move(newState), step+1)) return;
	// 				}
	// 			}
	// 		}
	// 	}
	// }




	//new efficient storage
	static int RunCombosOpt(StateOpt state) {
		std::deque<StateOpt> queue{state};
		std::deque<int> path{0};
		std::unordered_set<StateOpt, StateOptHash> unique;
		int fastestFinish = -1;
		int stateN = 0;
		while (true) {
			if (fastestFinish != -1)
				return fastestFinish;
			StateOpt front = queue.front();
			int step = path.front();
			queue.pop_front();
			path.pop_front();
			BranchStatesOpt(front, step, queue, unique, path, fastestFinish);
			stateN++;
		}
	}
	static void BranchStatesOpt(const StateOpt& state, int step, std::deque<StateOpt>& queue, std::unordered_set<StateOpt, StateOptHash>& states, std::deque<int>& path, int& fastestFinish) {
		auto checkPlaceState = [&](StateOpt& _state, int _step) -> bool {

			_state.Sort();
			if (_state.IsFinished()) {
				fastestFinish = _step;
				return true;
			}
			if (states.emplace(_state).second) {
				queue.emplace_back(_state);
				path.emplace_back(_step);
			}
			return false;
		};
		int i = 0;
		int current = state.Here();
		while (current != 0) {
			if ((current & 1) == 0) {
				current >>= 1;
				i++;
				continue;
			}
			int id = i >> 1;
			bool g = i & 1;
			//move UP - high priority
			if (state.elev < 3) {
				//prioritize moving 2 up
				int rest = current >> 1;
				int j = i+1;
				while (rest != 0) {
					if ((rest & 1) == 0) {
						rest >>= 1;
						j++;
						continue;
					}
					int id2 = j >> 1;
					bool g2 = (j & 1) == 1;
					StateOpt newState = state;
					newState.MoveUp(id, g, id2, g2, newState.elev);
					newState.elev++;
					if (newState.CheckSafety(newState.elev) && newState.CheckSafety(newState.elev - 1)) {
						if (checkPlaceState(newState, step+1)) return;
					}
					rest >>= 1;
					j++;
				}
				//move 1 up -> low prior
				{
					StateOpt newState = state;
					newState.MoveUp(id, g, newState.elev);
					newState.elev++;
					if (newState.CheckSafety(newState.elev) && newState.CheckSafety(newState.elev - 1)) {
						if (checkPlaceState(newState, step+1)) return;
					}
				}
			}
			//move DOWN low priority
			if (state.elev > 0) {
				//move down 1 high priority
				{
					StateOpt newState = state;
					newState.MoveDown(id, g, newState.elev);
					newState.elev--;
					if (newState.CheckSafety(newState.elev) && newState.CheckSafety(newState.elev + 1)) {
						if (checkPlaceState(newState, step+1)) return;
					}
				}
				//move down 2 low priority
				int rest = current >> 1;
				int j = i+1;
				while (rest != 0) {
					if ((rest & 1) == 0) {
						rest >>= 1;
						j++;
						continue;
					}
					int id2 = j >> 1;
					bool g2 = j & 1;
					StateOpt newState = state;
					newState.MoveDown(id, g, id2, g2, newState.elev);
					newState.elev--;
					if (newState.CheckSafety(newState.elev) && newState.CheckSafety(newState.elev + 1)) {
						if (checkPlaceState(newState, step+1)) return;
					}
					rest >>= 1;
					j++;
				}
			}
			current >>= 1;
			i++;
		}
	}
	static void run() {

		std::ifstream inputStream("2016/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int floor;
		StateOpt state;
		state.elev = 0;
		while (std::getline(inputStream, line))
		{
			char typeCh;
			int id;
			std::stringstream ss(line);
			ss >> floor;
			while (ss >> typeCh >> id) {
				state.Add(id, typeCh == 'G', floor-1);
			}
		}
		inputStream.close();
		int moves = RunCombosOpt(state);
		std::cout << moves << '\n';
	}
	static void runPart2() {
		std::ifstream inputStream("2016/11.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		int floor;
		StateOpt state;
		state.elev = 0;
		while (std::getline(inputStream, line))
		{
			char typeCh;
			int id;
			std::stringstream ss(line);
			ss >> floor;
			while (ss >> typeCh >> id) {
				state.Add(id, typeCh == 'G', floor-1);
			}
		}
		inputStream.close();
		state.Add(5, true, 0);
		state.Add(5, false, 0);
		state.Add(6, true, 0);
		state.Add(6, false, 0);
		int moves = RunCombosOpt(state);
		std::cout << moves << '\n';
	}
};

//-------------- NOTES AREA ----------------
/*

	input is simplified to this format
	1 M0 M1
	2 G0
	3 G1
	4

	bruteforce kinda works
	if we have N elements
	there are 4^(2N) cases for each of them at most around (2N)^2 new cases can be generated
	for 2 elements there are 4^4 = 256 which is not that bad
	for 5 elements it is 1 mil
	for 7 elements it is 300 mil
	after reducing expensive storages to binary operation it is still slow due to time complexity 16^N * N^2

	each id is stored as 2 bits (first is microchip, second is generator)
	and the entire floor state is 1 single integer, example:
	...10|00|10|11 means there are G0, M0, G1, G3 elements on this floor
	tho we dont really care which id is on which floor, we only care if they are the same or not so

	1 G0 G1
	2 M1
	3 M0
	4

	and

	1 G0 G1
	2 M0
	3 M1
	4

	are equivalent states (considering the elevator is on the same floor)
	assuming pairs are interchangable
	this reduces complexity by (N!)/N^2 (N^2 due to sorting)
	or to be boring (15+N)!/(15!*N!) * N^4
	so for 7 elements it only 200k cases instead of 300 mil, x1000 less but with x49 times tradeoff for sorting
*/