#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char sep)
{
	auto substr = str;
	std::vector<std::string> res;
	size_t pos = substr.find(sep);
	while (pos != std::string::npos) {
		res.push_back(substr.substr(0, pos));
		substr = substr.substr(pos + 1, substr.size() - pos - 1);
		pos = substr.find(sep);
	}
	if (!substr.empty())
		res.push_back(substr);
	return res;
}

std::vector<std::string> split(const std::string& str, const std::string& sep)
{
	auto substr = str;
	std::vector<std::string> res;
	size_t pos = substr.find(sep);
	while (pos != std::string::npos) {
		res.push_back(substr.substr(0, pos));
		substr = substr.substr(pos + sep.length(), substr.size() - pos - sep.length());
		pos = substr.find(sep);
	}
	if (!substr.empty())
		res.push_back(substr);

	return res;
}

template<typename T>
constexpr int digitsIn (T num) {
	int res = 0;
	while (num != 0) {
		res++;
		num /= 10;
	}
	return res;
}

template<typename T>
constexpr void Log(const std::vector<T>& arr) {
	for (const T& el : arr) {
		std::cout << el << " ";
	}
	std::cout << '\n';
}
template<typename T, typename U>
constexpr void Log(const std::unordered_map<T,U>& arr) {
	for (const auto& pair : arr) {
		std::cout << pair.first << ' ' << pair.second << '\n';
	}
	std::cout << '\n';
}

template<typename T>
bool inRange(T width, T height, T x, T y) {
	if (x < width && y < height && x >= 0 && y >= 0) {
		return  true;
	}
	return false;
}


const std::unordered_map<int, std::pair<int, int>> UtilFacingVec {
				{0, {1,0}},
				{1, {0,-1}},
				{2, {-1,0}},
				{3, {0,1}},
			};

std::vector<std::tuple<int, int, int>>::const_iterator UtilFindPlace(const std::vector<std::tuple<int, int, int>>& arr, int order) {
	for ( int i=0; i<arr.size(); i++ ) {
		if (order < std::get<0>(arr.at(i))) {
			return arr.cbegin() + i;
		}
	}
	return arr.end();
}

int UtilDijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, std::vector<std::pair<int,int>> starts, const int& endX, const int& endY) {
	//weight, x, y
	std::vector<std::tuple<int, int, int>> queue;

	for (auto& [x,y] : starts) {
		queue.emplace(UtilFindPlace(queue, weights[x][y]), weights[x][y], x, y);
	}
	while (!queue.empty()) {
		auto data = queue.begin();
		int& score = std::get<0>(*data);
		int& x = std::get<1>(*data);
		int& y = std::get<2>(*data);
		std::vector<std::tuple<int, int, int>> toAdd;

		for (int i=0; i<4; i++) {

			auto& [dx, dy] = UtilFacingVec.at(i);

			int nx = x + dx, ny = y + dy;

			if (!inRange(width, height, nx, ny)) {
				continue;
			}

			if (weights[nx][ny] == -1)
				continue;

			int weight = score + 1;
			if (weights[nx][ny] > weight) {
				weights[nx][ny] = weight;
				toAdd.emplace_back(weight, nx, ny);
			}
		}

		queue.erase(data);

		for (auto& t : toAdd) {
			int weight = std::get<0>(t);
			queue.emplace(UtilFindPlace(queue, weight), t);
		}
	}

	return weights[endX][endY];
}


int UtilDijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY) {

	weights[startX][startY] = 0;

	std::vector<std::pair<int,int>> starts {
		{startX, startY}
	};
	UtilDijkstra(weights, width, height, starts, endX, endY);

	return weights[endX][endY];
}