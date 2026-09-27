#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <regex>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "utils.h"

//snailfish numbers hell


struct Node {
	std::variant<Node*, int> left;
	std::variant<Node*, int> right;
	Node* parent;
	Node* getLeftNode() {
		auto p = std::get_if<Node*>(&left);
		return p ? *p : nullptr;
	}
	Node* getRightNode() {
		auto p = std::get_if<Node*>(&right);
		return p ? *p : nullptr;
	}
	const Node* getLeftNode() const {
		auto p = std::get_if<Node*>(&left);
		return p ? *p : nullptr;
	}
	const Node* getRightNode() const {
		auto p = std::get_if<Node*>(&right);
		return p ? *p : nullptr;
	}
	bool isRightNumber() const {
		return std::holds_alternative<int>(right);
	}
	bool isLeftNumber() const {
		return std::holds_alternative<int>(left);
	}
	int* getLeftNum() {
		return std::get_if<int>(&left);
	}
	int* getRightNum() {
		return std::get_if<int>(&right);
	}
	const int* getLeftNum() const {
		return std::get_if<int>(&left);
	}
	const int* getRightNum() const {
		return std::get_if<int>(&right);
	}
	int* getLeftmostInChildren() {
		Node* context = this;
		while (context != nullptr) {
			if (context->isLeftNumber()) return context->getLeftNum();
			context = context->getLeftNode();
		}
		return nullptr;
	}
	int* getRightmostInChildren() {
		Node* context = this;
		while (context != nullptr) {
			if (context->isRightNumber()) return context->getRightNum();
			context = context->getRightNode();
		}
		return nullptr;
	}
	void explodeNode() {
		if (!isLeftNumber() || !isRightNumber()) {
			return;
		}
		Node* leftN = this;

		int rVal = *getRightNum();
		int lVal = *getLeftNum();

		while (true) {
			if (leftN->parent == nullptr) break;
			if (leftN->parent->getRightNode() == leftN) {
				leftN = leftN->parent;
				if (leftN->isLeftNumber()) {
					*leftN->getLeftNum() += lVal;
				}
				else {
					*leftN->getLeftNode()->getRightmostInChildren() += lVal;
				}
				break;
			}
			if (leftN->parent->getLeftNode() == leftN) {
				leftN = leftN->parent;
			}
		}

		Node* rightN = this;
		while (true) {
			if (rightN->parent == nullptr) break;
			if (rightN->parent->getLeftNode() == rightN) {
				rightN = rightN->parent;
				if (rightN->isRightNumber()) {
					*rightN->getRightNum() += rVal;
				}
				else {
					auto rightTreeRoot = rightN->getRightNode();
					auto target = rightTreeRoot->getLeftmostInChildren();
					*target += rVal;
				}
				break;
			}
			if (rightN->parent->getRightNode() == rightN) {
				rightN = rightN->parent;
			}
		}

		if (parent->getLeftNode() == this) {
			parent->left = 0;
		}
		if (parent->getRightNode() == this) {
			parent->right = 0;
		}
	}

	void print() {
		std::cout << '[';
		if (isLeftNumber()) std::cout << *getLeftNum();
		else getLeftNode()->print();
		std::cout << ',';
		if (isRightNumber()) std::cout << *getRightNum();
		else getRightNode()->print();
		std::cout << ']';
	}
};

class SnailfishNum {
	bool checkSplit_step(Node* node) {

		if (node->isLeftNumber()) {
			if (*node->getLeftNum() > 9) {
				splitNum(node, node->getLeftNum());
				return false;
			}
		}
		else {
			if (!checkSplit_step(node->getLeftNode())) return false;
		}

		if (node->isRightNumber()) {
			if (*node->getRightNum() > 9) {
				splitNum(node, node->getRightNum());
				return false;
			}
		}
		else {
			if (!checkSplit_step(node->getRightNode())) return false;
		}

		return true;
	}
	bool checkExplosion_step(Node* node, std::vector<Node*>& stack) {

		if (stack.size() > 4 && node->isLeftNumber() && node->isRightNumber()) {
			node->explodeNode();
			return false;
		}

		if (!node->isLeftNumber()) {
			stack.emplace_back(node->getLeftNode());
			bool res = checkExplosion_step(node->getLeftNode(), stack);
			stack.pop_back();
			if (!res) return false;
		}

		if (!node->isRightNumber()) {
			stack.emplace_back(node->getRightNode());
			bool res = checkExplosion_step(node->getRightNode(), stack);
			stack.pop_back();
			if (!res) {
				return false;
			}
		}
		return true;
	}
	Node* cloneToMem(const SnailfishNum& other) {
		return cloneNodeToMem(other.root);
	}
	Node* cloneNodeToMem(const Node* other) {
		nodes.emplace_back(std::make_unique<Node>());
		auto newNode = nodes.back().get();
		if (other->isLeftNumber()) {
			newNode->left = *other->getLeftNum();
		}
		else {
			auto created = cloneNodeToMem(other->getLeftNode());
			created->parent = newNode;
			newNode->left = created;
		}
		if (other->isRightNumber()) {
			newNode->right = *other->getRightNum();
		}
		else {
			auto created = cloneNodeToMem(other->getRightNode());
			created->parent = newNode;
			newNode->right = created;
		}
		return newNode;
	}
	Node* createNodeFromString(const std::string& str, int& ptr) {

		nodes.emplace_back(std::make_unique<Node>());
		Node* node = nodes.back().get();
		if (str[ptr] == '[') {
			ptr++;
			auto created = createNodeFromString(str, ptr);
			created->parent = node;
			node->left = created;
		}
		else {
			int num = 0;
			while (std::isdigit(str[ptr])) {
				num = num*10 + (str[ptr++] - '0');
			}
			node->left = num;
		}
		ptr++; // for comma
		if (str[ptr] == '[') {
			ptr++;
			auto created = createNodeFromString(str, ptr);
			created->parent = node;
			node->right = created;
		}
		else {
			int num = 0;
			while (std::isdigit(str[ptr])) {
				num = num*10 + (str[ptr++] - '0');
			}
			node->right = num;
		}
		ptr++;
		return node;
	}
	long long countMagnitude(const Node* node) {
		auto getMagnitude = [this](const std::variant<Node*, int>& val) {
			if (std::holds_alternative<Node*>(val)) {
				return countMagnitude(std::get<Node*>(val));
			}
			return static_cast<long long>(std::get<int>(val));
		};
		return getMagnitude(node->left)*3 + getMagnitude(node->right)*2;
	}

	Node* root = nullptr;
	std::vector<std::unique_ptr<Node>> nodes;

public:

	const Node* getRoot() {
		return root;
	}
	explicit SnailfishNum(const std::string& str) {
		createFromString(str);
	}
	SnailfishNum(const SnailfishNum& other) {
		root = cloneNodeToMem(other.root);
	}
	void createFromString(const std::string& str) {
		nodes.clear();
		int ptr = 1;
		root = createNodeFromString(str, ptr);
	}
	void splitNum(Node* parent, const int* num) {
		nodes.emplace_back(std::make_unique<Node>());
		Node* newNode = nodes.back().get();
		newNode->left = *num/2;
		newNode->right = (*num+1)/2;
		newNode->parent = parent;
		if (parent->getLeftNum() == num) {
			parent->left = newNode;
			return;
		}
		if (parent->getRightNum() == num) {
			parent->right = newNode;
			return;
		}
	}

	bool checkSimpification() {
		std::vector<Node*> stack;
		stack.emplace_back(root);
		if (!checkExplosion_step(root, stack)) return false;
		return checkSplit_step(root);
	}

	void add(const SnailfishNum& other) {
		nodes.emplace_back(std::make_unique<Node>());

		Node* newRoot = nodes.back().get();
		Node* anotherRoot = cloneToMem(other);

		root->parent = newRoot;
		anotherRoot->parent = newRoot;

		newRoot->left = root;
		newRoot->right = anotherRoot;

		root = newRoot;
		while (!checkSimpification());
	}

	long long countMagnitude() {
		return countMagnitude(root);
	}

	void print() const {
		root->print();
		std::cout << std::endl;
	}
};
struct Task
{
	static inline std::vector<SnailfishNum> nums;


	static void run() {

		std::ifstream inputStream("2021/18.txt");

		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
			return;
		}

		std::string line;
		while (std::getline(inputStream, line))
		{
			nums.emplace_back(line);
		}
		inputStream.close();
		SnailfishNum num = nums[0];
		for (int i=1; i<nums.size(); i++) {
			num.add(nums[i]);
		}
		std::cout << num.countMagnitude() << std::endl;
	}
	static void runPart2() {
		long long maxMag = 0;
		for (int i=0; i<nums.size(); i++) {
			for (int j=0; j<nums.size(); j++) {
				if (i == j) continue;
				auto num1 = nums[i];
				num1.add(nums[j]);
				long long mag = num1.countMagnitude();
				if (maxMag < mag) maxMag = mag;
			}
		}
		std::cout << maxMag << std::endl;
	}
};

//-------------- NOTES AREA ----------------
/*

*/