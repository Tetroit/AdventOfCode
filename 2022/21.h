#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <map>
#include <map>
#include <thread>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//monkey calculator

struct Task
{
	struct Expr {
		std::string a;
		char op;
		std::string b;
	};
	struct Monke {
		explicit Monke(Expr val) {
			expr = val;
		}
		explicit Monke(long long val) {
			expr = val;
		}
		std::variant<long long, Expr> expr;
		[[nodiscard]] long long getVal(const std::unordered_map<std::string, Monke>& map) const {
			if (std::holds_alternative<Expr>(expr)) {
				auto& var = std::get<Expr>(expr);
				long long valA = map.at(var.a).getVal(map);
				long long valB = map.at(var.b).getVal(map);
				switch (var.op) {
					case '+': return valA + valB;
					case '-': return valA - valB;
					case '*': return valA * valB;
					case '/': return valA / valB;
					default: return 0;
				}
			}
			else {
				return std::get<long long>(expr);
			}
		}
		long long bwd(const std::unordered_map<std::string, Monke>& map, long long total, const std::string& x) {
			if (!std::holds_alternative<Expr>(expr)) return 0;
			auto& var = std::get<Expr>(expr);
			switch (var.op) {
				case '+': return x == var.a ? total - map.at(var.b).getVal(map) : total - map.at(var.a).getVal(map);
				case '-': return x == var.a ? total + map.at(var.b).getVal(map) : map.at(var.a).getVal(map) - total;
				case '*': return x == var.a ? total / map.at(var.b).getVal(map) : total / map.at(var.a).getVal(map);
				case '/': return x == var.a ? total * map.at(var.b).getVal(map) : map.at(var.a).getVal(map) / total;
				default: return 0;
			}
		}
	};
	static inline const std::regex exprPattern = std::regex(R"((\w+): (\w+) (.) (\w+))");
	static inline const std::regex numPattern = std::regex(R"((\w+): (\d+))");
	static inline std::unordered_map<std::string, Monke> monkes;
	static inline std::unordered_map<std::string, std::string> parent;
	static void run() {

		std::ifstream inputStream("2022/21.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			std::smatch match;
			if (std::regex_match(line, match, numPattern)) {
				monkes.emplace(match[1].str(), Monke(std::stoll(match[2].str())));
			}
			else if (std::regex_match(line, match, exprPattern)) {
				monkes.emplace(match[1].str(), Monke({match[2].str(), match[3].str()[0], match[4].str()}));
				parent.emplace(match[2].str(), match[1].str());
				parent.emplace(match[4].str(), match[1].str());
			}
		}
		inputStream.close();
		std::cout << monkes.at("root").getVal(monkes) << "\n";
	}
	static void runPart2() {
		std::vector<std::string> varPath = {};
		std::string var = "humn";
		while (var != "root") {
			varPath.push_back(var);
			var = parent.at(var);
		}

		auto x = varPath.back();
		varPath.pop_back();
		Monke& root = monkes.at("root");
		Expr& rootExpr = std::get<Expr>(root.expr);
		long long target = rootExpr.a == x ? monkes.at(rootExpr.b).getVal(monkes) : monkes.at(rootExpr.a).getVal(monkes);
		while (!varPath.empty()) {
			x = varPath.back();
			varPath.pop_back();
			target = monkes.at(parent[x]).bwd(monkes, target, x);
		}
		std::cout << target << "\n";
	}
};

//-------------- NOTES AREA ----------------
/*

*/