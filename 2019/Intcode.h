#pragma once
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

enum struct HaltCode {
	NONE = 0,
	EXIT = 1,
	WAIT = 2,
	ERROR = 3,
};

template <typename T>
struct Intcode {
	std::vector<T> program;
	std::vector<T> reserve;
	int ptr=0;
	int base=0;

	Intcode() = default;
	Intcode(std::ifstream& inputStream) {
		long long num;
		while (inputStream >> num)
		{
			inputStream.ignore(1);
			program.push_back(num);
		}
		inputStream.close();
	}

	void start() {
		reserve = program;
	}
	void end() {
		program = reserve;
		ptr = 0;
		base = 0;
	}
	static void decypher(int instruction, int& opcode, int& mode1, int& mode2, int& mode3) {
		opcode = instruction % 100;
		instruction /= 100;
		mode1 = instruction % 10;
		instruction /= 10;
		mode2 = instruction % 10;
		instruction /= 10;
		mode3 = instruction % 10;
	}
	T& get (T pos) {
		if (pos >= program.size()) {
			program.resize(pos + 1);
		}
		return program[pos];
	}
	T getByMode(T pos, int mode) {
		if (mode == 1) return pos;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	T& getRefByMode(T pos, int mode) {
		if (mode == 1) std::cerr << "Cannot write to rvalue" << std::endl;
		if (mode == 2) return get(base+pos);
		return get(pos);
	}
	bool executeUntilInput(std::deque<T>& input, std::deque<T>& output, HaltCode& haltCode) {
		if (get(ptr) % 100 == 3 && input.empty()) {
			haltCode = HaltCode::WAIT;
			return false;
		}
		return execute(input, output, haltCode);
	}
	bool execute(std::deque<T>& input, std::deque<T>& output, HaltCode& haltCode) {
		int mode1 = 0;
		int mode2 = 0;
		int mode3 = 0;
		int code = 0;
		decypher(get(ptr), code, mode1, mode2, mode3);
		if (code == 99) {
			haltCode = HaltCode::EXIT;
			return false;
		}
		switch (code) {
			case 1: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) + getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 2: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) * getByMode(get(ptr+2), mode2);
				ptr+=4;
				break;
			}
			case 3: {
				getRefByMode(get(ptr+1), mode1) = input.front();
				input.pop_front();
				ptr+=2;
				break;
			}
			case 4: {
				output.push_back(getByMode(get(ptr+1), mode1));
				ptr+=2;
				break;
			}
			case 5: {
				ptr = getByMode(get(ptr+1), mode1) != 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 6: {
				ptr = getByMode(get(ptr+1), mode1) == 0 ? getByMode(get(ptr+2), mode2) : ptr+3;
				break;
			}
			case 7: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) < getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 8: {
				getRefByMode(get(ptr+3), mode3) = getByMode(get(ptr+1), mode1) == getByMode(get(ptr+2), mode2) ? 1 : 0;
				ptr+=4;
				break;
			}
			case 9: {
				base += getByMode(get(ptr+1), mode1);
				ptr+=2;
				break;
			}
			default: {
				std::cout << "Unknown code: " << code << std::endl;
				haltCode = HaltCode::ERROR;
				return false;
			}
		}
		return true;
	}
};